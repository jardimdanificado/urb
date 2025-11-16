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

            defs.push(`protected ${id} {\\${formatted}}`);
            return `pointer(mem, ${id})`;
        }
    );

    if (defs.length > 0) {
        src = defs.join('\n') + '\n' + src;
    }

    return src;
}

// === REMOÇÃO DE COMENTÁRIOS ===
function removeComments(src) {
    return src.replace(/\/\/[^\n\r]*/g, '');
}

// === REORDENAÇÃO DE CHAMADAS ===
function reorderFunctionCalls(src, macros) {
    const forbidden = { ...macros, 'private': true, 'public': true, 'protected': true };

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

// === PROCESSAMENTO PRINCIPAL ===
function process(s) {
    let out = '';
    let i = 0;
    let protected_defs = [];

    while (i < s.length) {
        const remaining = s.substring(i);
        const match = remaining.match(/^(.*?)\b(public|private|protected)\b/s);

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

                    if (kw === 'public') {
                        out += `${name}:\n${proc}\n${name}_end:`;
                    } else if (kw === 'protected') {
                        protected_defs.push(`${name}_end goto\n${name}:\n${proc}\n${name}_end:`);
                    } else if (kw === 'private') {
                        out += `${name}_end goto\n${name}:\n${proc}\n${name}_end:`;
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

    if (protected_defs.length > 0) {
        out = protected_defs.join('\n') + '\n' + out;
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
    src = reorderFunctionCalls(src, macros);
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