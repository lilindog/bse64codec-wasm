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
    const res = await fetch("../dist/wasm.wasm");
    const _wasm_bytes = await res.arrayBuffer();
    const { instance: { exports: mod } } = await WebAssembly.instantiate(_wasm_bytes);

    /**
     * 编码
     * 
     * @param {ArrayBuffer | string} source 
     * @return {ResultDescriptor}
     */
    function encode (source) {
        const u8 = _bytes(source);
        const byteLen = u8.byteLength;

        const 
            in_point = mod.requestMemory(byteLen),
            out_point = mod.requestMemory(byteLen + byteLen / 2);

        if ([ in_point, out_point ].some(i => i < 0)) {
            throw "): wasm 层面内存申请出错！";
        }

        new Uint8Array(mod.memory.buffer).set(u8, in_point);
        console.time("c wasm");
        mod.base64_encode(in_point, out_point);
        console.timeEnd("c wasm");
        const resBytes = _readStringBytes(new Uint8Array(mod.memory.buffer), out_point);

        return new ResultDescriptor(
            resBytes.buffer,
            new TextDecoder().decode(resBytes),
        );
    }

    /**
     * 解码 
     * 
     * @param {ArrayBuffer | string} source
     * @return {ResultDescriptor}
     */
    function decode (source) {
        const u8 = _bytes(source);
        const byteLen = u8.byteLength;
        const 
            in_point = mod.requestMemory(byteLen),
            out_point = mod.requestMemory(byteLen);

        if ([ in_point, out_point ].some(i => i < 0)) {
            throw "): wasm 层面内存申请出错！";
        }
        new Uint8Array(mod.memory.buffer).set(u8, in_point);
        mod.base64_decode(in_point, out_point);
        const resBytes = _readStringBytes(new Uint8Array(mod.memory.buffer), out_point);
        return new ResultDescriptor(
            resBytes.buffer,
            new TextDecoder().decode(resBytes),
        );
    }

    return { encode, decode };
}

function _readStringBytes (bytes, point) {
    const _bytes = [];
    for (let i = point; bytes[i] !== 0; i++) {
        _bytes.push(bytes[i]);
    }
    return new Uint8Array(_bytes);
}

function _bytes (source) {
    try {
        return ({
            "[object ArrayBuffer]": () => new Uint8Array(source),
            "[object String]": () => new TextEncoder().encode(source),
        })[{}.toString.call(source)]();
    } catch {
        throw `): source 类型错误!`;
    }
}