---
title: Eclipse下Tomcat+Nginx+Java部署
date: 2018-07-01 23:15:00
description: 关于后端开发的一些经验与总结
categories:
 - share
tags: 
- tomcat
- nginx
---

[Tomcat]: https://tomcat.apache.org/download-90.cgi

[deployment_diagram]: https://raw.githubusercontent.com/Eros-L/Eros-L.github.io/master/_posts/image/deployment_diagram.png

[tomcat1]: https://raw.githubusercontent.com/Eros-L/Eros-L.github.io/master/_posts/image/tomcat/1.png
[tomcat2]: https://raw.githubusercontent.com/Eros-L/Eros-L.github.io/master/_posts/image/tomcat/2.png
[tomcat3]: https://raw.githubusercontent.com/Eros-L/Eros-L.github.io/master/_posts/image/tomcat/3.png
[tomcat4]: https://raw.githubusercontent.com/Eros-L/Eros-L.github.io/master/_posts/image/tomcat/4.png
[tomcat5]: https://raw.githubusercontent.com/Eros-L/Eros-L.github.io/master/_posts/image/tomcat/5.png
[tomcat6]: https://raw.githubusercontent.com/Eros-L/Eros-L.github.io/master/_posts/image/tomcat/6.png
[tomcat7]: https://raw.githubusercontent.com/Eros-L/Eros-L.github.io/master/_posts/image/tomcat/7.png
[tomcat8]: https://raw.githubusercontent.com/Eros-L/Eros-L.github.io/master/_posts/image/tomcat/8.png


## Eclipse下Tomcat+Nginx+Java部署

----------

### 1、前言
&emsp;这篇文章将要介绍如何在Mac OS 10.13.3下，通过Eclipse实现Tomcat 9.0.8 + jre 1.8 + Nginx的带有负载均衡的后端。在设计上，我最初的打算是通过Redis来实现MySQL的缓存，但由于组内分工的不明确和交流的缺乏，最终导致暂时没有时间完成这个设计。且外，由于个人基础不扎实以及开发人手不足，在搭建后端时并没有选择一个适当的操作系统，因此在使用docker构造容器上也仍存在不足，目前打算在学习压力减缓的时候再继续完善这方面的工作。最后，特别感谢Vector_dl对我的帮助，以下附上后端的部署图。

* 部署图
![][deployment_diagram]
<br />

### 2、Eclipse下配置Tomcat
&emsp;在刚入手后端开发时，Tomcat 9.0.10仍未发布，但由于Tomcat 9.0.8并不支持jre10，最终只能选择jre.18，特地在此声明。
&emsp;首先，我们需要在[Tomcat][Tomcat]的官网上获取我们需要的版本，直接解压，然后进行环境变量的设置，在此就不赘述了，详情可见下图，但需要注意的事，我们最好在解压时选择一个合适的路径，方便以后的开发。
![][tomcat1]
![][tomcat2]
![][tomcat3]
&emsp;在Eclipse的选用上，我们选择的是Eclipse Java EE IDE for Web Developers，我们需要在Eclipse中先新建一个Server，具体流程如下。
![][tomcat4]
![][tomcat5]
![][tomcat6]
&emsp;然后，我们可以开始创建我们的项目，我们直接在Java Resources下新建一个Servlet，Eclipse将自动帮我们完成部署。
![][tomcat7]
![][tomcat8]