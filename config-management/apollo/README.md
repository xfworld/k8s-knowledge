# [Apollo](https://github.com/ctripcorp/apollo)

Apollo 是携程开源的一个分布式配置中心，在国内非常流行。
它功能强大，支持配置的继承，也有提供 API 方便自动化配置。缺点是权限管理比较弱，也不支持信息加密，不适合直接存储敏感信息。


## 一、部署流程

Apollo 分布式配置中心，最简单的部署方式是使用 Kubernetes + Helm.
流程如下：

1. 创建数据库：Apollo服务端共需要两个数据库：ApolloPortalDB 和 ApolloConfigDB
   1. 详见：[Apollo 分布式部署指南-创建数据库](https://github.com/ctripcorp/apollo/wiki/%E5%88%86%E5%B8%83%E5%BC%8F%E9%83%A8%E7%BD%B2%E6%8C%87%E5%8D%97#21-%E5%88%9B%E5%BB%BA%E6%95%B0%E6%8D%AE%E5%BA%93)
2. 通过 Helm 进行部署：
   1. 部署 apollo-configservice 和 apollo-adminservice，每一个配置环境都需要独立部署这两个服务。
   2. 部署 apollo-portal，这个是 Apollo 的 UI/API 面板，所有环境公用。
   3. 详见[Apollo 分布式部署指南-kubernetes部署](https://github.com/ctripcorp/apollo/wiki/%E5%88%86%E5%B8%83%E5%BC%8F%E9%83%A8%E7%BD%B2%E6%8C%87%E5%8D%97#24-kubernetes%E9%83%A8%E7%BD%B2)

部署完成后就能直接用浏览器登入 portal 面板了，默认账号 apollo 密码 admin，helm 配置中没找到自定义密码的参数。

### 注意事项

1. 目前 Portral 貌似是使用服务端 Session，Cookie 只包含 SessionID，Session 具体的内容保存在服务端内存中。
   这导致 Portral 本身成了一个有状态应用，Session 不能在 Pod 之间共享！因此多副本情况下必须配置基于 ClientIP 的会话亲和！使用 Istio/Ingress 时还需要在 Istio/Ingress 配置中配上基于 ClientIP 的路由策略。


## 二、使用 Apollo

### 1. 添加配置

Apollo 支持通过 RESTful API 自动添加修改配置，也可以通过 Portal 面板手动添加修改配置。

手动添加的流程略过，下面介绍一下通过 RESTful API 自动化添加修改配置的方法。
它很适合用于搭建与刷新测试环境，不然一套微服务成百上千的配置项，不可能每次测试都去通过面板手动配置。

>RESTful API 的官方文档：[Apollo开放平台](https://github.com/ctripcorp/apollo/wiki/Apollo%E5%BC%80%E6%94%BE%E5%B9%B3%E5%8F%B0)

使用 Apollo Open API 需要先通过 Portal 面板创建一个 APP 并给予权限，这个流程也可以使用脚本直接调 Portal API 实现。


### 2. 微服务读取配置 

Apollo 为微服务提供 HTTP API 来读取与更新配置，并且支持以长轮询的方式动态更新微服务的配置。

官方及社区有提供各语言的 SDK，请自行在 Apollo WiKi 上查看。