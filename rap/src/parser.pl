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

# --- remove comentários estilo // até o fim da linha ---
$src =~ s{//[^\n\r]*}{}g;
# --- fim da remoção ---

# --- coleta macros no estilo "macro name { ... }" (corrigido) ---
my %macros;
pos($src) = 0;
while ($src =~ /\bmacro\s+([A-Za-z_][A-Za-z0-9_]*)\s*\{/gc) {
    my $name = $1;
    # posição do início do match ("m" de "macro")
    my $match_start = $-[0];
    # posição do '{' foi deixada em pos()-1
    my $open_pos = pos($src) - 1;
    my ($body, $pos_after) = extract_block($src, $open_pos, '{', '}');
    # remove ENTIRE declaration: desde $match_start até pos_after
    substr($src, $match_start, $pos_after - $match_start, '');
    # reposiciona o cursor ao ponto onde removemos (pra continuar scanning)
    pos($src) = $match_start;
    # armazena o corpo tal como veio (sem a palavra 'macro' nem o nome)
    $macros{$name} = $body;
}
# --- fim da coleta ---

# --- expande chamadas de macro (suporta name(arg,...) ou name arg1 arg2 ...) ---
for my $name (keys %macros) {
    my $body = $macros{$name};

    # repetimos a substituição até não haver mais ocorrências desta macro
    # (evita expandir parcialmente em contextos estranhos)
    1 while $src =~ s{
        \b\Q$name\E\b                       # nome da macro
        (?:                                  # dois estilos alternativos
           \s*\(\s*([^()]*?)\s*\)            # 1) parênteses: name(a,b,c)
         |                                   
           (?:\s+([^\n{};()]+(?:\s+[^\n{};()]+)*))  # 2) espaço-separado: name a b c (até newline/;/{/})
        )
    }{
        my $args_raw = defined $1 && length $1 ? $1 : $2;
        # se for estilo com vírgulas (parênteses): split por ','
        my @vals;
        if (defined $1 && length $1) {
            @vals = map { s/^\s+|\s+$//gr } split /,/, $args_raw, -1;
        } else {
            # estilo espaço-separado: remove espaço inicial e split por espaços
            $args_raw =~ s/^\s+//;
            @vals = map { s/^\s+|\s+$//gr } split /\s+/, $args_raw;
        }

        # faz as substituições de $1, $2
        my $exp = $body;
        for my $i (0 .. $#vals) {
            my $n = $i + 1;
            my $v = $vals[$i];
            # protege barras e $ na inserção? deixar simples: substitui literais $n
            $exp =~ s/\$\Q$n\E(?!\d)/$v/g;
        }

        # se houver placeholders não preenchidos, deixa vazio
        $exp =~ s/[\$]\d+\b//g;

        "$exp";   # opcional: envolver em parênteses para preservar precedência
    }egx;
}
# --- fim da expansão ---


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

# --- conversão de strings "..." para pointers + defs automáticos ---
{
    my @defs;
    my $counter = 0;

    $src =~ s{
        "                                  # abre aspas
        (                                  # captura conteúdo
            (?: \\. | [^"\\] | \n )*       # aceita escapes, texto normal e quebras de linha
        )
        "                                  # fecha aspas
    }{
        my $content = $1;
        my $id = "_unamed_string$counter";
        $counter++;

        # formata conteúdo
        $content =~ s/\n/\\n/g;
        $content =~ s/ /\\s/g;

        push @defs, "${id} [\\$content]";

        "pointer(mem, $id)";
    }egx;

    if (@defs) {
        # insere as definições no início do arquivo
        $src = join("\n", @defs) . "\n" . $src;
    }
}
# --- fim da conversão ---

# --- conversão de strings `...` para \... (sem barra final) ---
$src =~ s{
    `                                   # abre crase
    (                                   # captura conteúdo
        (?: \\. | [^`\\] | \n )*        # aceita escapes, texto normal e quebras de linha
    )
    `                                   # fecha crase
}{
    my $str = $1;
    # converte quebras de linha em \n
    $str =~ s/\n/\\n/g;
    # converte espaços em \s
    $str =~ s/ /\\s/g;
    # adiciona só a barra inicial
    "\\$str"
}egx;
# --- fim da conversão ---

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
        if ($s =~ /\G(.*?)\b(scope|if|while|until|local)\b/sgc) {
            $out .= $1;           # texto até a keyword
            my $kw = $2;

            if ($kw eq 'scope' or $kw eq 'local') {
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
