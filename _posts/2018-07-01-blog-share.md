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
[Nginx]: http://nginx.org/en/download.html

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

[nginx1]: https://raw.githubusercontent.com/Eros-L/Eros-L.github.io/master/_posts/image/nginx/1.png
[nginx2]: https://raw.githubusercontent.com/Eros-L/Eros-L.github.io/master/_posts/image/nginx/2.png
[nginx3]: https://raw.githubusercontent.com/Eros-L/Eros-L.github.io/master/_posts/image/nginx/3.png
[nginx4]: https://raw.githubusercontent.com/Eros-L/Eros-L.github.io/master/_posts/image/nginx/4.png
[nginx5]: https://raw.githubusercontent.com/Eros-L/Eros-L.github.io/master/_posts/image/nginx/5.png
[nginx6]: https://raw.githubusercontent.com/Eros-L/Eros-L.github.io/master/_posts/image/nginx/6.png
[nginx7]: https://raw.githubusercontent.com/Eros-L/Eros-L.github.io/master/_posts/image/nginx/7.png


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

>> 在本教程里，我们需要用到的jar包如下: <br />
>> [MySQL Connector/J](https://dev.mysql.com/downloads/connector/j/) <br />
>> [Gson](http://repo1.maven.org/maven2/com/google/code/gson/gson/2.8.5/) <br />

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

### 4、配置Nginx
&emsp;考虑到实际情况，我们无需在Eclipse中导入Nginx亦可完成负载均衡的实现，因此，接下来的教材将在终端中完成搭建。我们可以在[Nginx][Nginx]官网上获得我们需要的Nginx，在安装Nginx前，我们需要先安装它的依赖PCRE。我们将下载好的Nginx解压，然后再其目录下完成下列操作。若配置成功，在浏览器中前往127.0.0.1:80将会看到nginx的主页。<br />
![][nginx1]
<br />
![][nginx2]
<br />
![][nginx3]
<br />
![][nginx4]
<br />
![][nginx5]
<br />
![][nginx6]
<br />

&emsp;接下来，我们将通过修改/nginx-1.14.0/conf/下的nginx.conf文件实现服务器分流。首先，我们要将#user nobody;改为user root owner;以赋予nginx权限。为了防止监听接口冲突，我们将server中的listen改为8888。location的配置则是对url中的内容进行匹配，匹配的格式有很多种，但我们在这里都不需要用到，只需将所有的地址转向我们的真实服务器即可，其中，proxy_pass http://pml_backend;则是此处的关键语句。最后，我们对upstream进行配置，在upstream中添加我们搭建好的tomcat服务器，然后通过weight，对其权重赋值(默认为1)。至此，我们的搭建已全部完成。<br />
```

user  root owner; 
worker_processes  1;

#error_log  logs/error.log;
#error_log  logs/error.log  notice;
#error_log  logs/error.log  info;

#pid        logs/nginx.pid;


events {
    worker_connections  1024;
}


http {
    include       mime.types;
    default_type  application/octet-stream;

    #log_format  main  '$remote_addr - $remote_user [$time_local] "$request" '
    #                  '$status $body_bytes_sent "$http_referer" '
    #                  '"$http_user_agent" "$http_x_forwarded_for"';

    #access_log  logs/access.log  main;

    sendfile        on;
    #tcp_nopush     on;

    #keepalive_timeout  0;
    keepalive_timeout  65;

    #gzip  on;

    upstream pml_backend {
        server 172.18.146.154:8080 weight=1;
        server 172.18.146.136:8080 weight=3;
    }

    server {
        listen       8888;

        #charset koi8-r;

        #access_log  logs/host.access.log  main;

        location / {
            proxy_pass http://pml_backend;
            proxy_redirect off;
            proxy_set_header Host $host;
            proxy_set_header X-Real-IP $remote_addr;
            proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
            client_max_body_size 10m;
            client_body_buffer_size 128k;
            proxy_connect_timeout 90;
            proxy_read_timeout 90;
            proxy_buffer_size 4k;
            proxy_buffers 6 32k;
            proxy_busy_buffers_size 64k;
            proxy_temp_file_write_size 64k;
        }

        #error_page  404              /404.html;

        # redirect server error pages to the static page /50x.html
        #
        error_page   500 502 503 504  /50x.html;
        location = /50x.html {
            root   html;
        }

        # proxy the PHP scripts to Apache listening on 127.0.0.1:80
        #
        #location ~ \.php$ {
        #    proxy_pass   http://127.0.0.1;
        #}

        # pass the PHP scripts to FastCGI server listening on 127.0.0.1:9000
        #
        #location ~ \.php$ {
        #    root           html;
        #    fastcgi_pass   127.0.0.1:9000;
        #    fastcgi_index  index.php;
        #    fastcgi_param  SCRIPT_FILENAME  /scripts$fastcgi_script_name;
        #    include        fastcgi_params;
        #}

        # deny access to .htaccess files, if Apache's document root
        # concurs with nginx's one
        #
        #location ~ /\.ht {
        #    deny  all;
        #}
    }


    # another virtual host using mix of IP-, name-, and port-based configuration
    #
    #server {
    #    listen       8000;
    #    listen       somename:8080;
    #    server_name  somename  alias  another.alias;

    #    location / {
    #        root   html;
    #        index  index.html index.htm;
    #    }
    #}


    # HTTPS server
    #
    #server {
    #    listen       443 ssl;
    #    server_name  localhost;

    #    ssl_certificate      cert.pem;
    #    ssl_certificate_key  cert.key;

    #    ssl_session_cache    shared:SSL:1m;
    #    ssl_session_timeout  5m;

    #    ssl_ciphers  HIGH:!aNULL:!MD5;
    #    ssl_prefer_server_ciphers  on;

    #    location / {
    #        root   html;
    #        index  index.html index.htm;
    #    }
    #}

}
```
<br />
![][nginx7]
<br />

### 5、总结
&emsp;想要获得更多的参考，可以访问[项目的开源github](https://github.com/OrderingService/OrderingSystemBackend)查看源码。但是由于项目完成度的问题，项目的docker可能存在问题，有时间的话将会尽快进行修正，望包容。