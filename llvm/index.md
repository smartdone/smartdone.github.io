## LLVM

### build

- [llvm release](http://releases.llvm.org)

```shell
mkdir llvm
cd llvm
git clone --depth 1 http://llvm.org/git/llvm.git
cd llvm/tools
git clone --depth 1 http://llvm.org/git/clang.git
git clone --depth 1 http://llvm.org/git/compiler-rt.git
git clone --depth 1 http://llvm.org/git/libcxx.git
git clone --depth 1 http://llvm.org/git/libcxxabi.git
cd ../..
mkdir build
cd build
cmake ../
make -j16
```

## 使用说明

1. [llvm pass](./llvm_pass.md)

## reference

libclang、libTooling
官方参考:<https://clang.llvm.org/docs/Tooling.html>
应用:语法树分析、语言转换等

Clang插件开发
官方参考
1、<https://clang.llvm.org/docs/ClangPlugins.html>
2、<https://clang.llvm.org/docs/ExternalClangExamples.html>
3、<https://clang.llvm.org/docs/RAVFrontendAction.html>
应用:代码检查(命名规范、代码规范)等

Pass开发
官方参考:<https://llvm.org/docs/WritingAnLLVMPass.html>
应用:代码优化、代码混淆等

开发新的编程语言
1、 <https://llvm-tutorial-cn.readthedocs.io/en/latest/index.html>
2、<https://kaleidoscope-llvm-tutorial-zh-cn.readthedocs.io/zh_CN/latest/>

作者：Coffee_LaFa

链接：https://www.jianshu.com/p/1367dad95445

來源：简书

简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。