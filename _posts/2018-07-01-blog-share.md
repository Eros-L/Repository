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
[tomcat9]: https://raw.githubusercontent.com/Eros-L/Eros-L.github.io/master/_posts/image/tomcat/9.png
[tomcat10]: https://raw.githubusercontent.com/Eros-L/Eros-L.github.io/master/_posts/image/tomcat/10.png
[tomcat11]: https://raw.githubusercontent.com/Eros-L/Eros-L.github.io/master/_posts/image/tomcat/11.png
[tomcat12]: https://raw.githubusercontent.com/Eros-L/Eros-L.github.io/master/_posts/image/tomcat/12.png


## Eclipse下Tomcat+Nginx+Java部署

----------

### 1、前言
&emsp;这篇文章将要介绍如何在Mac OS 10.13.3下，通过Eclipse实现Tomcat 9.0.8 + jre 1.8 + Nginx的带有负载均衡的后端。在设计上，我最初的打算是通过Redis来实现MySQL的缓存，但由于组内分工的不明确和交流的缺乏，最终导致暂时没有时间完成这个设计。且外，由于个人基础不扎实以及开发人手不足，在搭建后端时并没有选择一个适当的操作系统，因此在使用docker构造容器上也仍存在不足，目前打算在学习压力减缓的时候再继续完善这方面的工作。最后，特别感谢Vector_dl对我的帮助，以下附上后端的部署图。

* 部署图
![][deployment_diagram]
<br />

### 2、Eclipse下配置Tomcat
&emsp;在刚入手后端开发时，Tomcat 9.0.10仍未发布，但由于Tomcat 9.0.8并不支持jre10，最终只能选择jre.18，特地在此声明。<br />
&emsp;首先，我们需要在[Tomcat][Tomcat]的官网上获取我们需要的版本，直接解压，然后进行环境变量的设置，在此就不赘述了，详情可见下图，但需要注意的事，我们最好在解压时选择一个合适的路径，方便以后的开发。<br />
![][tomcat1]
<br />
![][tomcat2]
<br />
![][tomcat3]
<br />
&emsp;在Eclipse的选用上，我们选择的是Eclipse Java EE IDE for Web Developers，我们需要在Eclipse中先新建一个Server，具体流程如下。<br />
![][tomcat4]
<br />
![][tomcat5]
<br />
![][tomcat6]
<br />
&emsp;然后，我们可以开始创建我们的项目，我们直接在Java Resources下新建一个Servlet，Eclipse将自动帮我们完成部署。<br />
![][tomcat7]
<br />
![][tomcat8]
<br />
### 3、JDBC
&emsp;到此，我们已经完成了Tomcat的配置，接下来，我们需要实现Java数据库连接(Java Database Connectivity)。<br />

>> 在本教程里，我们需要用到的jar包如下:
>> [MySQL Connector/J](https://dev.mysql.com/downloads/connector/j/)
>> [Gson](http://repo1.maven.org/maven2/com/google/code/gson/gson/2.8.5/)

<br />

&emsp;我们首先要封装一个类，在类中实现数据库连接的封装，连接的相关教材可在网上寻找，这一部分的重点在于对Json格式的处理。我们从数据库中获得菜单后，需要将数据转换成json格式传回前端，利用Gson包中的JsonObject与JsonArray可以帮助我们方便地生成Json格式的数据。<br />
```
JsonArray array = new JsonArray();
if (result == null) {
	return null;
}
try {
	ResultSetMetaData metaData = result.getMetaData();
	while (result.next()) {
		JsonObject tmp = new JsonObject();
		String columnName;
		for (int i = 1; i < metaData.getColumnCount(); ++i) {
			columnName = metaData.getColumnName(i+1);
			tmp.addProperty(columnName, result.getFloat(i+1));
		}
		array.add(tmp);
	}
} catch (Exception e) {
	e.printStackTrace();
}
return array.toString();
```
<br />
&emsp;在Servlet的实现方面，以Select为例，实际上只是简单的html request。<br />
```
String res = "";	
String function = request.getParameter("func");
if (function.equals("getMenu")) {
	res = DButil.getMenu();
} else if (function.equals("getOrder")) {
	res = DButil.getOrder();
}
```
<br />
&emsp;最后，我们也需要解析从前端传回的Json数据，在此，我们借用Gson包中提供的方法可以简单地实现这一功能。其中，我们需要实现一个Order类，类中的成员变量名需要对应Json数据中的键名。<br />
```
boolean res = false;
String function = request.getParameter("func");
if (function.equals("createOrder")) {
	Order order = gson.fromJson(request.getReader(), Order.class);
	res = DBUtil.createOrder(order);
	response.getWriter().append(gson.toJson(order));
}
```
<br />
### 3、运行Tomcat
&emsp;完成上述任务后，我们终于可以在Tomcat上运行我们的后端，我们只需要在刚刚创建的Server上对我们的项目选择Run即可，具体过程可参照下图。注意，为了让我们的后端能在局域网中正常访问，我们在创建Server时需要将名字设为我们的IP地址。<br />
![][tomcat9]
<br />
![][tomcat10]
<br />
![][tomcat11]
<br />
![][tomcat12]
<br />