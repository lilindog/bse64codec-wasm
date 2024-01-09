"use strict";

const { runCmds } = require("./comm");
const fs = require("node:fs");
const path = require("node:path");
const parseenv = require("parseenv");
const { MEMORY, O, WASM_FILE_NAME, BYTES_LIB_NAME } = parseenv("./build.env+");

main()
.catch(console.error);

async function main () {
    const isOk = await runCmds([
        `emcc ./src/base64.c ./src/string.c ./src/memory.c -O${O} -s STANDALONE_WASM --no-entry -s TOTAL_MEMORY=${MEMORY} -o ./dist/${WASM_FILE_NAME}`,
    ]);
    if (!isOk) throw "\n): emcc编译错误，请参考输出日志排错！\n";
    console.log("emcc编译通过！");
    // c构建wasm较小，不考虑流式
    const bytes = fs.readFileSync(path.resolve(__dirname, `../dist/${WASM_FILE_NAME}`));
    let bytesStr = bytes.reduce((r, code) => {
        r += code + ",";
        return r;
    }, "export default [") + "];";
    fs.writeFileSync(path.resolve(__dirname, `../dist/${BYTES_LIB_NAME}`), bytesStr);
    console.log("\n(: 构建成功！\n");
}