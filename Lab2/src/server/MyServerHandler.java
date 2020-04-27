package server;

import com.alibaba.fastjson.JSONObject;
import io.netty.buffer.ByteBuf;
import io.netty.buffer.Unpooled;
import io.netty.channel.ChannelFutureListener;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.ChannelInboundHandlerAdapter;
import io.netty.handler.codec.http.*;
import io.netty.util.CharsetUtil;

/**
 * @author csqure
 */
public class MyServerHandler extends ChannelInboundHandlerAdapter {

    @Override
    public void channelActive(ChannelHandlerContext ctx) throws Exception {
        System.out.println("已连接上"+ctx.channel().localAddress().toString());
    }

    @Override
    public void channelInactive(ChannelHandlerContext ctx) throws Exception {
        System.out.println("客户端断开链接" + ctx.channel().localAddress().toString());
    }

    @Override
    public void channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {
        if(msg instanceof FullHttpRequest){
            FullHttpRequest req = (FullHttpRequest)msg;
            try {
                // 1.获取URI
                String uri = req.uri();
                // 2.获取请求体
                ByteBuf buf = req.content();
                String content = buf.toString(CharsetUtil.UTF_8);
                // 3.获取请求方法
                HttpMethod method = req.method();
                // 4.获取请求头
                HttpHeaders headers = req.headers();
                // 5.根据method，确定不同的逻辑

                if(method.equals(HttpMethod.GET)){

                    // TODO 
                }

                if(method.equals(HttpMethod.POST)){

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
                if(method.equals(HttpMethod.PUT)){
                    ErrorAnswer(ctx,"PUT");
                }
                if(method.equals(HttpMethod.DELETE)){
                    ErrorAnswer(ctx,"DELETE");
                }
                if(method.equals(HttpMethod.OPTIONS)){
                    ErrorAnswer(ctx,"OPTIONS");
                }
                if(method.equals(HttpMethod.HEAD)){
                    ErrorAnswer(ctx,"HEAD");
                }
                if(method.equals(HttpMethod.TRACE)){
                    ErrorAnswer(ctx,"TRACE");
                }
                if(method.equals(HttpMethod.CONNECT)){
                    ErrorAnswer(ctx,"CONNECT");
                }
                else{
                    postNotFoundError(ctx);
                }
            } finally {
                req.release();
            }
        }
    }

    private void response(ChannelHandlerContext ctx, Content c) {
        // 1.设置响应
        FullHttpResponse resp = new DefaultFullHttpResponse(
                HttpVersion.HTTP_1_1,
                HttpResponseStatus.OK,
                Unpooled.copiedBuffer(JSONObject.toJSONString(c), CharsetUtil.UTF_8));

        resp.headers().set(HttpHeaderNames.CONTENT_TYPE, "text/html; charset=UTF-8");

        // 2.发送
        // 注意必须在使用完之后，close channel
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
    private void ErrorAnswer(ChannelHandlerContext ctx,String d){
        String c= new String();
        c=      "\r\n<html><title>501 Not Implemented</title><body bgcolor=ffffff>"+
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
    
    class Content{
        String uri;
        String content;
        String Name;
        String ID;

        public String getUri() {
            return uri;
        }

        public void setUri(String uri) {
            this.uri = uri;
        }

        public String getContent() {
            return content;
        }

        public void setContent(String content) {
            this.content = content;
        }

        public String getName() {
            return Name;
        }

        public void setName(String name) {
            Name = name;
        }

        public String getID() {
            return ID;
        }

        public void setID(String ID) {
            this.ID = ID;
        }
    }
}
