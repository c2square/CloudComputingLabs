import java.io.IOException;
import java.io.InputStream;
import java.net.ServerSocket;
import java.net.Socket;

public class test {
    public static void main(String[] args) {
        try {
            ServerSocket serverSocket= new ServerSocket(8800);
            Socket socket = serverSocket.accept();
            InputStream inputStream= socket.getInputStream();
//            byte[] by = new byte[1024];
            int size=inputStream.available();
            byte[] buffer=new byte[size];
            inputStream.read(buffer);
            String request=new String(buffer);
            System.out.println(request);
            //5:关闭套接字
            socket.close();

        } catch (IOException e) {
            e.printStackTrace();
        }

    }
}
