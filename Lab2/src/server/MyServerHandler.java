package server;

import io.netty.buffer.ByteBuf;
import io.netty.buffer.Unpooled;
import io.netty.channel.*;
import io.netty.handler.codec.http.*;
import io.netty.util.CharsetUtil;

import java.io.File;
import java.io.IOException;
import java.io.RandomAccessFile;

/**
 * @author csqure
 */
public class MyServerHandler extends SimpleChannelInboundHandler<FullHttpRequest> {

    @Override
    public void channelActive(ChannelHandlerContext ctx) throws Exception {
        System.out.println("已连接上"+ctx.channel().localAddress().toString());
    }

    @Override
    public void channelInactive(ChannelHandlerContext ctx) throws Exception {
        System.out.println("客户端断开链接" + ctx.channel().localAddress().toString());
    }

    @Override
    public void channelRead0(ChannelHandlerContext ctx, FullHttpRequest request) throws Exception {
            // 1.获取URI
            String uri = request.uri();

            // 2.获取请求体
            ByteBuf buf = request.content();
            String content = buf.toString(CharsetUtil.UTF_8);

            // 3.获取请求方法
            HttpMethod method = request.method();


                if(method.equals(HttpMethod.GET)){
                    if("/".equals(uri)){
                            uri+="index.html";
                            String ext=uri.substring(uri.lastIndexOf(".")+1);//取后缀
                            uri=uri.substring(1);
                            File html = new File(uri);
                            getFileMethod(ctx,html,ext);
                    }else{//请求路径的解析
                        String ext=uri.substring(uri.lastIndexOf(".")+1);//取后缀
                        uri=uri.substring(1);//去除前面的“/”
                            File file = new File(uri);
                            if(file.exists()){
                                if (!file.isFile()){
                                    file = new File(uri+"/index.html");
                                    if (!file.exists()){
                                        GetNotFoundError(ctx,uri);
                                    }
                                }
                                getFileMethod(ctx,file,ext);
                            }
                            else{
                                GetNotFoundError(ctx,uri);
                            }
                    }
                }
                else if(method.equals(HttpMethod.POST)){

                if (!("/Post_show".equals(uri))){
                    postNotFoundError(ctx);
                    System.out.println("uri error "+uri);
                }else {
                    String[] tmp0=content.split("&");
                    if (tmp0.length!=2){
                        postNotFoundError(ctx);
                        System.out.println("val length error"+content);
                    }else {
                        String[] NameAndVal=tmp0[0].split("=");
                        String[] IDAndVal=tmp0[1].split("=");
                        if ("Name".equals(NameAndVal[0])&&"ID".equals(IDAndVal[0])){
                            PostHandleSuccess(ctx, NameAndVal[1],IDAndVal[1]);

                        }else {
                            postNotFoundError(ctx);
                            System.out.println("val  error "+NameAndVal[0]+"  "+IDAndVal[0]);
                        }
                    }
                }

        }

            else{
                ErrorMethod(ctx,method.name());
            }
        }


    @Override
    public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) throws Exception {
        System.out.println(cause);
    }

    private void getFileMethod(ChannelHandlerContext ctx, File html,String etc) throws IOException {

        RandomAccessFile file= new RandomAccessFile(html, "r");
        HttpResponse response = new DefaultHttpResponse(HttpVersion.HTTP_1_1, HttpResponseStatus.OK);

        response.headers().set(HttpHeaderNames.SERVER, "lib'2 Web Server");
        if(etc.equals("html")){
            response.headers().set(HttpHeaderNames.CONTENT_TYPE, "text/html; charset=UTF-8");
        }else if(etc.equals("jpg")||etc.equals("png")){
            if(etc.equals("jpg")){
                response.headers().set(HttpHeaderNames.CONTENT_TYPE, "image/jpg");
            }else if(etc.equals("png")){
                response.headers().set(HttpHeaderNames.CONTENT_TYPE, "image/png");
            }
        }else if(etc.equals("js")){
            response.headers().set(HttpHeaderNames.CONTENT_TYPE, "application/x-javascript");
        }else if(etc.equals("css")){
            response.headers().set(HttpHeaderNames.CONTENT_TYPE, "text/css; charset=UTF-8");
        }
        response.headers().set(HttpHeaderNames.CONTENT_LENGTH, file.length());

        ctx.write(response);
        ctx.write(new DefaultFileRegion(file.getChannel(), 0, file.length()));
        ChannelFuture future = ctx.writeAndFlush(LastHttpContent.EMPTY_LAST_CONTENT);
        future.addListener(ChannelFutureListener.CLOSE);
        file.close();
    }

    private void GetNotFoundError(ChannelHandlerContext ctx,String url){
        String c= "\r\n<html><title>404 Not Found</title><body bgcolor=ffffff>"+
                "\r\n Not Found"+
                "\r\n<p>Couldn't find this file:  ./"+url+"/index.html"+
                "\r\n<hr><em>HTTP Web server</em>"+
                "\r\n</body></html>";
        FullHttpResponse resp = new DefaultFullHttpResponse(
                HttpVersion.HTTP_1_1,
                HttpResponseStatus.NOT_FOUND,
                Unpooled.copiedBuffer(c,CharsetUtil.UTF_8));
        resp.headers().set(HttpHeaderNames.SERVER, "lib'2 Web Server");
        resp.headers().set(HttpHeaderNames.CONTENT_TYPE, "text/html");
        resp.headers().set(HttpHeaderNames.CONTENT_LANGUAGE, c.length());
        ctx.writeAndFlush(resp).addListener(ChannelFutureListener.CLOSE);
    }

    private void PostHandleSuccess(ChannelHandlerContext ctx,String Name,String ID){
        String c= "\r\n<html><title>POST method</title><body bgcolor=ffffff>"+
                "\r\n Your Name:    "+Name+
                "\r\n ID:    "+ID+
                "\r\n<hr><em>HTTP Web server</em>"+
                "\r\n</body></html>";

        FullHttpResponse resp = new DefaultFullHttpResponse(
                HttpVersion.HTTP_1_1,
                HttpResponseStatus.OK,
                Unpooled.copiedBuffer(c,CharsetUtil.UTF_8));
        resp.headers().set(HttpHeaderNames.SERVER, "lib'2 Web Server");
        resp.headers().set(HttpHeaderNames.CONTENT_TYPE, "text/html");
        resp.headers().set(HttpHeaderNames.CONTENT_LANGUAGE, c.length());

        ctx.writeAndFlush(resp).addListener(ChannelFutureListener.CLOSE);
    }

    private void postNotFoundError(ChannelHandlerContext ctx){
        String c= "\r\n<html><title>404 Not Found</title><body bgcolor=ffffff>"+
                "\r\n Not Found"+
                "\r\n<hr><em>HTTP Web server</em>"+
                "\r\n</body></html>";
        FullHttpResponse resp = new DefaultFullHttpResponse(
                HttpVersion.HTTP_1_1,
                HttpResponseStatus.NOT_FOUND,
                Unpooled.copiedBuffer(c,CharsetUtil.UTF_8));
        resp.headers().set(HttpHeaderNames.SERVER, "lib'2 Web Server");
        resp.headers().set(HttpHeaderNames.CONTENT_TYPE, "text/html");
        resp.headers().set(HttpHeaderNames.CONTENT_LANGUAGE, c.length());
        ctx.writeAndFlush(resp).addListener(ChannelFutureListener.CLOSE);
    }

    private void ErrorMethod(ChannelHandlerContext ctx, String d){
        String c="\r\n<html><title>501 Not Implemented</title><body bgcolor=ffffff>"+
                "\r\n Not Implemented"+
                "\r\n<p>Does not implement this method: "+d+
                "\r\n<hr><em>HTTP Web server</em>"+
                "\r\n</body></html>";
        FullHttpResponse resp = new DefaultFullHttpResponse(
                HttpVersion.HTTP_1_1,
                HttpResponseStatus.NOT_IMPLEMENTED,
                Unpooled.copiedBuffer(c,CharsetUtil.UTF_8));
        resp.headers().set(HttpHeaderNames.SERVER, "lib'2 Web Server");
        resp.headers().set(HttpHeaderNames.CONTENT_TYPE, "text/html");
        resp.headers().set(HttpHeaderNames.CONTENT_LANGUAGE, c.length());
        ctx.writeAndFlush(resp).addListener(ChannelFutureListener.CLOSE);
    }
    
}
