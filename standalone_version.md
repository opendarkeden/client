
## 下载客户端


1. 客户端数据文件

百度网盘 https://pan.baidu.com/s/1-DufSEmnydMbOtTwOo_h8A 提取码 6bcl

或者 Mediafire https://www.mediafire.com/file/017bif66kyieviw/DARKEDEN.zip/file

2. 修改 `DARKEDEN/Data/Info/GameClient.inf` 文件内容，单机版里应该是 `127.0.0.1`

3. DARKEDEN 目录下有几个 bat 文件，点击运行，将以 '窗口模式' 或者 '全屏模式' 启动游戏

## 安装服务端

下载并安装 Docker Desktop

https://docs.docker.com/desktop/install/windows-install/


下载打包好的服务端 docker 镜像


```
docker pull tiancaiamao/darkeden
```

下载 mysql 的 docker 镜像


```
docker pull mysql/mysql-server:5.7
```

将下面内容保存为 docker-compose.yml 文件


```
version: '3'

services:

  odk-mysql:
    image: mysql/mysql-server:5.7
    volumes:
      - ../initdb:/docker-entrypoint-initdb.d/
    environment:
        MYSQL_ROOT_PASSWORD: 123456
    restart: on-failure
    command: mysqld --sql_mode="ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION"
    networks:
      - odk-network

  odk-server:
    image: tiancaiamao/darkeden:latest
    build:
      context: ..
      dockerfile: Dockerfile
    command: ["sleep","infinity"]
    ports:
      - "9999:9999"
      - "9998:9998"
      - "9997:9997"
      - "9997:9997/udp"
    networks:
      - odk-network

networks:
  odk-network:
```


启动 docker 镜像


```
docker-compose -f docker-compose.yml up -d
```

启动服务器

```
docker exec -it docker_odk-server_1  ./start.sh
```

如何关闭


```sh
docker exec -it docker_odk-server_1  ./stop.sh
docker-compose down
```
