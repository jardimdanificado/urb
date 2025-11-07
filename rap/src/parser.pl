#!/usr/bin/env perl
use strict;
use warnings;
use feature 'say';

# unified.pl (corrigido)
# Lê todo o arquivo e transforma constructs:
#  - scope NAME { ... }  -> scope({{NAME}}, {{ ... }})
#  - while(cond) { ... } -> while({{cond}}, {{ ... }})
#  - until(cond) { ... } -> until({{cond}}, {{ ... }})
#  - if(cond) { ... } else { ... } -> if({{cond}}, {{then}}, {{else}})
#
# Principais diferenças: usa apenas pos($s)/\G para controle de leitura,
# evitando duplicação e inconsistências.

local $/;
my $src = <>;
$src = '' unless defined $src;

sub add_scope_and_local {
    my ($src) = @_;
    my $out = '';
    my $i = 0;
    my $len = length $src;
    my $in_s = 0;
    my $quote = '';

    while ($i < $len) {
        my $ch = substr($src, $i, 1);

        # strings
        if (!$in_s && ($ch eq '"' || $ch eq "'")) {
            $in_s = 1; $quote = $ch; $out .= $ch; $i++; next;
        } elsif ($in_s) {
            if ($ch eq '\\') { $out .= substr($src, $i, 2); $i += 2; next; }
            if ($ch eq $quote) { $in_s = 0; $quote = ''; $out .= $ch; $i++; next; }
            $out .= $ch; $i++; next;
        }

        # ignora enum { ... }
        if (substr($src, $i) =~ /\A(enum)\s*\{/) {
            $out .= $&; $i += length($&); next;
        }

        # ignora local nome { ... }
        if (substr($src, $i) =~ /\Alocal\s+([A-Za-z_][A-Za-z0-9_]*)\s*\{/) {
            $out .= $&; $i += length($&); next;
        }

        # ignora scope nome { ... }
        if (substr($src, $i) =~ /\Ascope\s+([A-Za-z_][A-Za-z0-9_]*)\s*\{/) {
            $out .= $&; $i += length($&); next;
        }

        # caso de "Nome {" → scope Nome {
        if (substr($src, $i) =~ /\A([A-Za-z_][A-Za-z0-9_]*)\s*\{/) {
            my $name = $1;
            $out .= "scope $name {";
            $i += length($&);
            next;
        }

        # caso de "Nome [" → local Nome {
        if (substr($src, $i) =~ /\A([A-Za-z_][A-Za-z0-9_]*)\s*\[/) {
            my $name = $1;
            $out .= "local $name {";
            $i += length($&);
            next;
        }

        # substitui ']' por '}'
        if ($ch eq ']') {
            $out .= '}';
            $i++;
            next;
        }

        $out .= $ch;
        $i++;
    }

    return $out;
}
# --- conversão de strings "..." para \... (sem barra final) ---
$src =~ s{
    "                                  # abre aspas
    (                                  # captura conteúdo
        (?: \\. | [^"\\] | \n )*       # aceita escapes, texto normal e quebras de linha
    )
    "                                  # fecha aspas
}{
    my $str = $1;
    # converte quebras de linha em \n
    $str =~ s/\n/\\n/g;
    # converte espaços em \s
    $str =~ s/ /\\s/g;
    # remove aspas internas, se houver
    $str =~ s/"//g;
    # adiciona só a barra inicial
    "\\$str"
}egx;
# --- fim da conversão ---

# --- remove comentários estilo // até o fim da linha ---
$src =~ s{//[^\n\r]*}{}g;
# --- fim da remoção ---

$src = add_scope_and_local($src);

# garantir que pos começa em zero
pos($src) = 0;

print process($src);

sub process {
    my ($s) = @_;
    my $out = '';

    # assegura ponteiro no início para esta string localmente
    pos($s) = 0;
    my $len = length $s;

    while (pos($s) < $len) {
        if ($s =~ /\G(.*?)\b(scope|struct|if|while|until|local|enum)\b/sgc) {
            $out .= $1;           # texto até a keyword
            my $kw = $2;

            if ($kw eq 'scope' or $kw eq 'local' or $kw eq 'struct') {
                my $kind = $kw;
                $s =~ /\G\s*/gc;
                if ($s =~ /\G([A-Za-z0-9_]+)\s*/gc) {
                    my $name = $1;
                    $s =~ /\G\s*/gc;
                    if ($s =~ /\G\{/gc) {
                        my ($inner, $pos_after) = extract_block($s, pos($s)-1, '{', '}');
                        pos($s) = $pos_after;
                        my $proc = process($inner);
                        $out .= "$kind({{$name}}, {{\n$proc\n}})";
                    } else {
                        $out .= "$kind $name";
                    }
                } else {
                    $out .= $kind;
                }
            }
            elsif ($kw eq 'if') {
                $s =~ /\G\s*/gc;
                if ($s =~ /\G\(/gc) {
                    my ($cond, $pos_after_cond) = extract_block($s, pos($s)-1, '(', ')');
                    pos($s) = $pos_after_cond;
                    $s =~ /\G\s*/gc;
                    if ($s =~ /\G\{/gc) {
                        my ($then_block, $pos_after_then) = extract_block($s, pos($s)-1, '{', '}');
                        pos($s) = $pos_after_then;
                        # optional else
                        my $else_block = '';
                        $s =~ /\G\s*/gc;
                        if ($s =~ /\Gelse\b/ogc) {
                            $s =~ /\G\s*/gc;
                            if ($s =~ /\G\{/gc) {
                                ($else_block, $pos_after_then) = extract_block($s, pos($s)-1, '{', '}');
                                pos($s) = $pos_after_then;
                            } else {
                                # else sozinho: roll back to before "else" so it remains literal
                                pos($s) = pos($s) - length('else');
                            }
                        }
                        my $proc_then = process($then_block);
                        my $proc_else = $else_block ? process($else_block) : '';
                        $out .= "if({{${cond}}}, {{\n$proc_then\n}}, {{\n$proc_else\n}})";
                    } else {
                        $out .= "if(${cond})";
                    }
                } else {
                    $out .= 'if';
                }
            }
            elsif ($kw eq 'while' or $kw eq 'until') {
                my $kind = $kw;
                $s =~ /\G\s*/gc;
                if ($s =~ /\G\(/gc) {
                    my ($cond, $pos_after_cond) = extract_block($s, pos($s)-1, '(', ')');
                    pos($s) = $pos_after_cond;
                    $s =~ /\G\s*/gc;
                    if ($s =~ /\G\{/gc) {
                        my ($body, $pos_after_body) = extract_block($s, pos($s)-1, '{', '}');
                        pos($s) = $pos_after_body;
                        my $proc_body = process($body);
                        $out .= "$kind({{${cond}}}, {{\n$proc_body\n}})";
                    } else {
                        $out .= "$kind($cond)";
                    }
                } else {
                    $out .= $kind;
                }
            } elsif ($kw eq 'enum') {
                $s =~ /\G\s*/gc;
                if ($s =~ /\G\{/gc) {
                    my ($inner, $pos_after) = extract_block($s, pos($s)-1, '{', '}');
                    pos($s) = $pos_after;
                    my $proc = process($inner);
                    $out .= "enum(\n$proc\n)";
                } else {
                    $out .= 'enum';
                }
            } else {
                $out .= $kw;
            }
        } else {
            # copia o que sobrou (a partir da posição atual do ponteiro)
            my $p = pos($s) // 0;
            $out .= substr($s, $p);
            last;
        }
    }

    return $out;
}

# extrai bloco balanceado começando no caractere de abertura indicado.
# recebe a string inteira $s, posição do open char (offset), char_open, char_close
# retorna (conteúdo_interno, pos_depois_do_close)
sub extract_block {
    my ($s, $openpos, $open, $close) = @_;
    my $len = length $s;
    my $i = $openpos;
    my $depth = 0;
    my $start_inner;
    my $in_string = 0;
    my $strchar = '';

    while ($i < $len) {
        my $ch = substr($s, $i, 1);

        # tratar strings para não contar chaves/parens dentro delas
        if ($in_string) {
            if ($ch eq '\\') {
                $i += 2; next;
            }
            if ($ch eq $strchar) {
                $in_string = 0;
                $strchar = '';
            }
            $i++;
            next;
        } else {
            if ($ch eq '"' or $ch eq "'") {
                $in_string = 1;
                $strchar = $ch;
                $i++;
                next;
            }
        }

        if ($ch eq $open) {
            $depth++;
            $start_inner = $i + 1 unless defined $start_inner;
        } elsif ($ch eq $close) {
            $depth--;
            if ($depth == 0) {
                my $inner = defined $start_inner ? substr($s, $start_inner, $i - $start_inner) : '';
                my $pos_after_close = $i + 1;
                return ($inner, $pos_after_close);
            }
        }
        $i++;
    }

    # se não encontrou fechamento balanceado, retorna resto como inner e pos final
    my $inner = defined $start_inner ? substr($s, $start_inner) : '';
    return ($inner, $len);
}
