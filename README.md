# base64编解码器

由c语言实现的WebAssembly base64编解码器。   

# 预览  
`npm run preview` -> 浏览器打开 http://127.0.0.1:8080/examples/wasm.html


# 集成
项目引入即可，采用esm模块系统。  

# API
```ts
  type ResultDescriptor = {
    string: string;
    buffer: ArrayBuffer;
  }
  
  declare module "base64codec-wasm" {
    export default function (): Promise<{ 
        encode: (source: ArrayBuffer | string) => ResultDescriptor; 
        decode: (source: ArrayBuffer | string) => ResultDescriptor;
    }>;
  }
```
