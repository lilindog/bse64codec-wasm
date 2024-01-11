import wasm_bytes from "./wasm-bytes.js";

class _Descriptor {
    constructor (...args) {
        const { FIELDS } = new.target;
        return FIELDS.reduce((r, f, i) => {
            Object.defineProperty(r, f, {
                writable: false,
                enumerable: false,
                configurable: false,
                value: args[i],
            });
            return r;
        }, Object.create(null));
    }
}

export class ResultDescriptor extends _Descriptor {
    static FIELDS = [
        "buffer",
        "string",
    ];
}

export default async function () {
    const { instance: { exports: mod } } = await WebAssembly.instantiate(new Uint8Array(wasm_bytes));
    return { encode, decode };

    /**
     * 编码
     * 
     * @param {ArrayBuffer | string} source 
     * @return {ResultDescriptor}
     */
    function encode (source) {
        const [ u8, byteLen, in_point, out_point ] = _prehandle(mod, source);
        new Uint8Array(mod.memory.buffer).set(u8, in_point);
        __BASE64CODEC_WASM_DEBUG__ && console.time("c ine");
        const en_len = mod.base64_encode(in_point, byteLen, out_point);
        __BASE64CODEC_WASM_DEBUG__ && console.timeEnd("c ine");
        const resUi8 = new Uint8Array(mod.memory.buffer, out_point, en_len);
        mod.releaseMemory(in_point);
        mod.releaseMemory(out_point);
        return new ResultDescriptor(
            resUi8,
            new TextDecoder().decode(resUi8),
        );
    }

    /**
     * 解码 
     * 
     * @param {ArrayBuffer | string} source
     * @return {ResultDescriptor}
     */
    function decode (source) {
        const [ u8, byteLen, in_point, out_point ] = _prehandle(mod, source);
        new Uint8Array(mod.memory.buffer).set(u8, in_point);
        __BASE64CODEC_WASM_DEBUG__ && console.time("c ind");
        const de_len = mod.base64_decode(in_point, byteLen, out_point);
        __BASE64CODEC_WASM_DEBUG__ && console.timeEnd("c ind");
        const resUi8 = new Uint8Array(mod.memory.buffer, out_point, de_len);
        mod.releaseMemory(in_point);
        mod.releaseMemory(out_point);
        return new ResultDescriptor(
            resUi8,
            new TextDecoder().decode(resUi8),
        );
    }
}

function _prehandle (mod, source) {
    const u8 = _bytes(source);
    const byteLen = u8.byteLength;
    const 
        in_point = mod.requestMemory(byteLen),
        out_point = mod.requestMemory(byteLen);

    if ([ in_point, out_point ].some(i => i < 0)) {
        throw "): wasm 层面内存申请出错！";
    }

    return [ u8, byteLen, in_point, out_point ];
}

function _bytes (source) {
    try {
        return ({
            "[object Uint8Array]": () => source,
            "[object String]": () => new TextEncoder().encode(source),
        })[{}.toString.call(source)]();
    } catch {
        throw `): source 类型错误!`;
    }
}