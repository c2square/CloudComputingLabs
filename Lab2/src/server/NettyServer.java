package server;

import io.netty.bootstrap.ServerBootstrap;
import io.netty.channel.ChannelFuture;
import io.netty.channel.ChannelOption;
import io.netty.channel.EventLoopGroup;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.nio.NioServerSocketChannel;

/**
 * @author csqure
 */
public class NettyServer {

    public static void main(String[] args) {
        new NettyServer().bing(args[0],new Integer(args[1]),new Integer(args[2]));
    }

    private void bing(String addr,int port,int threadNum) {
        //配置服务端NIO线程组
        int father=1,son=1;
        if(threadNum>1){
            father=threadNum/2;
            son=(threadNum+1)/2;
        }
        EventLoopGroup parentGroup = new NioEventLoopGroup(father);
        EventLoopGroup childGroup = new NioEventLoopGroup(son);
        try {
            ServerBootstrap b = new ServerBootstrap();
            b.group(parentGroup, childGroup)
                    .channel(NioServerSocketChannel.class)
                    .option(ChannelOption.SO_BACKLOG, Integer.MAX_VALUE)
                    .childHandler(new MyChannelInitializer());
            ChannelFuture f = b.bind(addr,port).sync();

            f.channel().closeFuture().sync();
        } catch (InterruptedException e) {
            e.printStackTrace();
        } finally {
            childGroup.shutdownGracefully();
            parentGroup.shutdownGracefully();
        }
    }
}
