# git操作指南

### 每次上传前(在vscode中):

1. 在vscode左侧工具栏第三个功能(样子像分叉的分支)里

    1. 可以看到自己对源代码的更改
    2. 对自己的更改按"+"号,代表你要保存这个更改

2. git stash

    >将本地工作保存在 缓存中

3. git pull

    > 将云端文件拉下来

4. git stash pop

    > 从缓存的本地文件弹出来,与云端文件做对比

5. 处理本地与云端文件的冲突

6. git commit -m "信息" 

    >  将当前工作打包并加入修改信息

7. git push

    > 把成品上传到云端

### 其他注意
windows的1903以及以后的版本，遇到VMware的小于15.1的版本时，从Windows复制文件到Ubuntu会出现卡机情况，去官网下载新版VMware
所需要的key可以参考
https://www.52pojie.cn/thread-801613-1-1.html