// ============================================
// CORE PREPROCESSOR - Motor de Processamento
// ============================================
// Sem dependências de ambiente (Browser/Node/QuickJS)

// === EXTRAÇÃO DE BLOCO BALANCEADO ===
function extractBlock(src, openpos, open, close) {
    let i = openpos;
    let depth = 0;
    let startInner = null;
    let inString = false;
    let strChar = '';

    while (i < src.length) {
        let ch = src[i];

        if (inString) {
            if (ch === '\\') {
                i += 2;
                continue;
            }
            if (ch === strChar) {
                inString = false;
                strChar = '';
            }
            i++;
            continue;
        } else {
            if (ch === '"' || ch === "'") {
                inString = true;
                strChar = ch;
                i++;
                continue;
            }
        }

        if (ch === open) {
            depth++;
            if (startInner === null) {
                startInner = i + 1;
            }
        } else if (ch === close) {
            depth--;
            if (depth === 0) {
                const inner = startInner !== null 
                    ? src.substring(startInner, i) 
                    : '';
                const posAfterClose = i + 1;
                return [inner, posAfterClose];
            }
        }
        i++;
    }

    const inner = startInner !== null 
        ? src.substring(startInner) 
        : '';
    return [inner, src.length];
}

// === COLETA DE MACROS ===
function collectMacros(src) {
    const macros = {};
    const macroRegex = /\bmacro\s+([A-Za-z_][A-Za-z0-9_]*)\s*\{/g;
    let match;

    const matches = [];
    while ((match = macroRegex.exec(src)) !== null) {
        matches.push({
            name: match[1],
            matchStart: match.index,
            openPos: match.index + match[0].length - 1
        });
    }

    for (let j = matches.length - 1; j >= 0; j--) {
        const m = matches[j];
        const [body, posAfter] = extractBlock(src, m.openPos, '{', '}');
        macros[m.name] = body;
        src = src.substring(0, m.matchStart) + src.substring(posAfter);
    }

    return [macros, src];
}

// === EXPANSÃO DE MACROS ===
function expandMacros(src, macros) {
    for (const name of Object.keys(macros)) {
        const body = macros[name];
        let changed = true;

        while (changed) {
            changed = false;
            const pattern = new RegExp(
                `\\b${escapeRegex(name)}\\b(?:\\s*\\(\\s*([^()]*)\\s*\\)|(?:\\s+([^\\n{};()]+(?:\\s+[^\\n{};()]+)*)))`,
                'g'
            );

            src = src.replace(pattern, (match, p1, p2) => {
                changed = true;
                const argsRaw = (p1 !== undefined && p1.length > 0) ? p1 : p2;
                
                let vals = [];
                if (p1 !== undefined && p1.length > 0) {
                    vals = p1.split(',').map(v => v.trim());
                } else {
                    vals = argsRaw.trim().split(/\s+/);
                }

                let exp = body;
                exp = exp.replace(/\$0\b/g, name);

                for (let i = 0; i < vals.length; i++) {
                    const n = i + 1;
                    const v = vals[i];
                    exp = exp.replace(new RegExp(`\\$${n}(?!\\d)`, 'g'), v);
                }

                exp = exp.replace(/\$\d+\b/g, '');

                return exp;
            });
        }
    }

    return src;
}

// === CONVERSÃO DE STRINGS "..." ===
function convertDoubleQuoteStrings(src) {
    const defs = [];
    let counter = 0;

    src = src.replace(
        /"((?:\\.|[^"\\]|\n)*)"/g,
        (match, content) => {
            const id = `_unamed_string${counter}`;
            counter++;

            let formatted = content
                .replace(/\n/g, '\\n')
                .replace(/ /g, '\\s');

            defs.push(`${id} [\\${formatted}]`);
            return `pointer(mem, ${id})`;
        }
    );

    if (defs.length > 0) {
        src = defs.join('\n') + '\n' + src;
    }

    return src;
}

// === CONVERSÃO DE STRINGS `...` ===
function convertBacktickStrings(src) {
    src = src.replace(
        /`((?:\\.|[^`\\]|\n)*)`/g,
        (match, content) => {
            let str = content
                .replace(/\n/g, '\\n')
                .replace(/ /g, '\\s');
            return `\\${str}`;
        }
    );

    return src;
}

// === REMOÇÃO DE COMENTÁRIOS ===
function removeComments(src) {
    return src.replace(/\/\/[^\n\r]*/g, '');
}

// === REORDENAÇÃO DE CHAMADAS ===
function reorderFunctionCalls(src, macros) {
    const forbidden = { ...macros, 'local': true, 'scope': true };

    let changed = true;
    while (changed) {
        changed = false;
        src = src.replace(
            /\b([A-Za-z_][A-Za-z0-9_]*)\s*\(\s*([^()]*?)\s*\)/g,
            (match, name, argsRaw) => {
                if (forbidden[name]) {
                    return match;
                }

                changed = true;
                const args = argsRaw.split(',').map(a => a.trim());
                args.reverse();
                args.push(name);

                return args.join(' ');
            }
        );
    }

    return src;
}

// === ADICIONAR SCOPE E LOCAL ===
function addScopeAndLocal(src) {
    let out = '';
    let i = 0;
    let inString = false;
    let quote = '';

    while (i < src.length) {
        const ch = src[i];

        if (!inString && (ch === '"' || ch === "'")) {
            inString = true;
            quote = ch;
            out += ch;
            i++;
            continue;
        } else if (inString) {
            if (ch === '\\') {
                out += src.substring(i, i + 2);
                i += 2;
                continue;
            }
            if (ch === quote) {
                inString = false;
                quote = '';
                out += ch;
                i++;
                continue;
            }
            out += ch;
            i++;
            continue;
        }

        const localMatch = src.substring(i).match(/^local\s+([A-Za-z_][A-Za-z0-9_]*)\s*\{/);
        if (localMatch) {
            out += localMatch[0];
            i += localMatch[0].length;
            continue;
        }

        const scopeMatch = src.substring(i).match(/^scope\s+([A-Za-z_][A-Za-z0-9_]*)\s*\{/);
        if (scopeMatch) {
            out += scopeMatch[0];
            i += scopeMatch[0].length;
            continue;
        }

        const nameOpenMatch = src.substring(i).match(/^([A-Za-z_][A-Za-z0-9_]*)\s*\{/);
        if (nameOpenMatch) {
            const name = nameOpenMatch[1];
            out += `scope ${name} {`;
            i += nameOpenMatch[0].length;
            continue;
        }

        const nameOpenBracketMatch = src.substring(i).match(/^([A-Za-z_][A-Za-z0-9_]*)\s*\[/);
        if (nameOpenBracketMatch) {
            const name = nameOpenBracketMatch[1];
            out += `local ${name} {`;
            i += nameOpenBracketMatch[0].length;
            continue;
        }

        if (ch === ']') {
            out += '}';
            i++;
            continue;
        }

        out += ch;
        i++;
    }

    return out;
}

// === PROCESSAMENTO PRINCIPAL ===
function process(s) {
    let out = '';
    let i = 0;

    while (i < s.length) {
        const remaining = s.substring(i);
        const match = remaining.match(/^(.*?)\b(scope|local)\b/s);

        if (match) {
            const before = match[1];
            const kw = match[2];
            out += before;
            i += match[0].length;

            const nameMatch = s.substring(i).match(/^\s*([A-Za-z0-9_]+)\s*/);
            if (nameMatch) {
                const name = nameMatch[1];
                i += nameMatch[0].length;

                const blockMatch = s.substring(i).match(/^\s*\{/);
                if (blockMatch) {
                    i += blockMatch[0].length;
                    const openPos = i - 1;
                    const [inner, posAfter] = extractBlock(s, openPos, '{', '}');
                    i = posAfter;

                    const proc = process(inner);

                    if (kw === 'scope') {
                        out += `${name}:\n${proc}\n${name}_end:`;
                    } else {
                        out += `${name}_end 1 jif\n${name}:\n${proc}\n${name}_end:`;
                    }
                } else {
                    out += `${kw} ${name}`;
                }
            } else {
                out += kw;
            }
        } else {
            out += s.substring(i);
            break;
        }
    }

    return out;
}

function escapeRegex(str) {
    return str.replace(/[.*+?^${}()|[\]\\]/g, '\\$&');
}

// === FUNÇÃO PÚBLICA PRINCIPAL ===
function preprocessCode(input) {
    let src = input;

    src = removeComments(src);
    const [macros, srcAfterMacros] = collectMacros(src);
    src = srcAfterMacros;
    src = expandMacros(src, macros);
    src = convertDoubleQuoteStrings(src);
    src = convertBacktickStrings(src);
    src = reorderFunctionCalls(src, macros);
    src = addScopeAndLocal(src);
    src = process(src);

    return src;
}

// Export para diferentes ambientes
if (typeof module !== 'undefined' && module.exports) {
    // Node.js / CommonJS
    module.exports = { preprocessCode };
} else if (typeof exports !== 'undefined') {
    // Browser / QuickJS
    exports.preprocessCode = preprocessCode;
}