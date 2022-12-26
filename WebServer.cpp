#include "WebServer.h"
#include "Client.h"

DWORD EventTotal = 0;
WSAEVENT EventArray[WSA_MAXIMUM_WAIT_EVENTS];
SOCKET_INFORMATION Clients[WSA_MAXIMUM_WAIT_EVENTS];
SOCKET_INFORMATION SelectedClient;

static DWORD WINAPI Worker(void*);

WebServer::WebServer(){ }

void WebServer::listen(const char port[]){
    int iResult;
    WSADATA wsaData;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return;
    }

    struct addrinfo *result = NULL, *ptr = NULL, hints;

    ZeroMemory(&hints, sizeof (hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the local address and port to be used by the server
    iResult = getaddrinfo(NULL, port, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        return;
    }

    SOCKET ListenSocket = INVALID_SOCKET;
    // Create a SOCKET for the server to listen for client connections

    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (ListenSocket == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return;
    }

    // Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return;
    }

    freeaddrinfo(result);

    if (::listen( ListenSocket, SOMAXCONN ) == SOCKET_ERROR ) {
        printf( "Listen failed with error: %ld\n", WSAGetLastError() );
        closesocket(ListenSocket);
        WSACleanup();
        return;
    }

    printf("Server is up running on port: %s\n", port);

    if((EventArray[0] = WSACreateEvent()) == WSA_INVALID_EVENT){
        printf("WSACreateEvent() failed with error %d\n", WSAGetLastError());
    }

    EventTotal = 1;

    CreateThread(NULL,0,Worker,(void*) this,0,&WORKER_ID);

    while(TRUE){
        SOCKET ClientSocket = INVALID_SOCKET;

        // Accept a client socket
        ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET) {
            printf("accept failed: %d\n", WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
            return;
        }

        Clients[EventTotal].Socket = ClientSocket;
        ZeroMemory(&(Clients[EventTotal].Overlapped), sizeof(WS_OVERLAPPED));
        Clients[EventTotal].BytesSEND = 0;
        Clients[EventTotal].BytesRECV = 0;
        Clients[EventTotal].DataBuf.len = DEFAULT_BUFLEN;
        Clients[EventTotal].DataBuf.buf = Clients[EventTotal].Buffer;

        if((Clients[EventTotal].Overlapped.hEvent = EventArray[EventTotal] = WSACreateEvent()) == WSA_INVALID_EVENT){
            printf("WSACreateEvent() failed with error %d\n", WSAGetLastError());
        }

        DWORD Flags = 0;
        ZeroMemory(Clients[EventTotal].DataBuf.buf, Clients[EventTotal].DataBuf.len);
        if(WSARecv(Clients[EventTotal].Socket, &(Clients[EventTotal].DataBuf), 1, &(Clients[EventTotal].BytesRECV), &Flags, &(Clients[EventTotal].Overlapped), NULL) == SOCKET_ERROR){
            if(WSAGetLastError() != WSA_IO_PENDING){
                printf("WSARecv() failed with error %d\n", WSAGetLastError());
            }
        }

        EventTotal += 1;

        // Signal the first event in the event array to tell the worker thread to
        // service an additional event in the event array
        if(WSASetEvent(EventArray[0]) == FALSE){
            printf("WSASetEvent() failed with error %d\n", WSAGetLastError());
        }
    }
    return;
}

WebServer::~WebServer(){
    return;
}

void WebServer::use_static_path(char* path){
    static_paths.append(String(path));
}

void WebServer::get(char* path, callback call){
    String key("GET:");
    key.push(path);
    // register a callback for that path
    handlers.set( key, call);
}

void WebServer::handle(String key, HttpRequest* req, HttpResponse* res){
    // NOTE: this is not a good way to extract a path from a String
    char* str = key.get();
    while(*(str++) != ':'){ }
    str++;
    String path(str);

    bool found = false;
    LinkedListIterator<LinkedList<String>::Bucket> iterator(static_paths.get_head());
    while(!iterator.is_end()){
        if(path.startWith(iterator.node()->value)){
            found = true;
            break;
        }
        iterator.increment();
    }
    if(found){
        // readfiles and send response
        // Open a handle to the file
         HANDLE hFile = CreateFile(path.get(), GENERIC_READ,0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

         if (hFile == INVALID_HANDLE_VALUE) {
             printf("\nINVALID_HANDLE_VALUE %d\n", GetLastError());
           // An error occurred
             res->status(404);
             res->setHeader("Server","abdelfetah-dev");
             res->send("<h1>404 Not Found!</h1>");
             return;
         }

         // Determine the size of the file
         DWORD fileSize = GetFileSize(hFile, NULL);

         // Allocate a buffer to hold the file contents
         char* buffer = new char[fileSize+1];

         // Read the contents of the file
         DWORD bytesRead;
         bool success = ReadFile(hFile, buffer, fileSize, &bytesRead, nullptr);

         if (!success) {
             printf("\nReadFile failed! %d\n", GetLastError());
           // An error occurred
             res->status(404);
             res->setHeader("Server","abdelfetah-dev");
             res->send("<h1>404 Not Found!</h1>");
             return;
         }

         // TODO: implement a way to map file extensions on its mimeType.

         // determine a mimeType by its extension.
        String ext(path.get() + path.indexOf('.'));
        ext.to_lower_case();
        // .png
        bool is_set = false;
        if(ext.startWith(String(".png"))){
            res->setHeader("Content-Type","image/png");
            is_set = true;
        }

        // .jpg, .jpeg, .jfif, .pjpeg, .pjp
        if(ext.startWith(String(".jpg")) || ext.startWith(String(".jpeg")) || ext.startWith(String(".jfif")) || ext.startWith(String(".pjpeg")) || ext.startWith(String(".pjp"))){
            res->setHeader("Content-Type","image/jpeg");
            is_set = true;
        }

        if(ext.startWith(String(".html"))){
            res->setHeader("Content-Type", "text/html");
            is_set = true;
        }

        if(ext.startWith(String(".js"))){
            res->setHeader("Content-Type", "text/javascript");
            is_set = true;
        }

        if(ext.startWith(String(".json"))){
            res->setHeader("Content-Type", "application/json");
            is_set = true;
        }

        if(ext.startWith(String(".mp4"))){
            res->setHeader("Content-Type","video/mp4");
            is_set = true;
        }

        // if files exist with unkonws mediaType.
        if(!is_set){
            res->setHeader("Content-Type","application/octet-stream");
            is_set = true;
        }

        res->status(200);
        res->send(buffer, fileSize);

        // Free the buffer
        delete[] buffer;

        // Close the handle to the file
        CloseHandle(hFile);
    }else{
        callback call;
        bool exist = handlers.get(key, call);
        if(exist)
            call(req,res);
        else{
            res->status(404);
            res->setHeader("Server","abdelfetah-dev");
            res->send("<h1>404 Not Found!</h1>");
        }
    }
}

DWORD WINAPI Worker(void* arg){
    WebServer* Server = (WebServer*) arg;
    DWORD Index;
    DWORD BytesTransferred = 0;
    DWORD Flags = 0;
    DWORD RecvBytes = 0, SendBytes = 0;
    while(TRUE){
        if((Index =  WSAWaitForMultipleEvents(EventTotal, EventArray, FALSE,  WSA_INFINITE, FALSE)) == WSA_WAIT_FAILED){
            printf("WSAWaitForMultipleEvents() failed %d\n", WSAGetLastError());
            return 0;
        }

        // If the event triggered was zero then a connection attempt was made on our listening socket.
        if((Index - WSA_WAIT_EVENT_0) == 0){
            WSAResetEvent(EventArray[0]);
            continue;
        }

        SelectedClient = Clients[Index - WSA_WAIT_EVENT_0];
        WSAResetEvent(EventArray[Index - WSA_WAIT_EVENT_0]);

        Client client(Clients[Index - WSA_WAIT_EVENT_0].Socket);

        //printf("recv_length: %d\n[ data ] ==========\n%s\n", SelectedClient.BytesRECV, SelectedClient.DataBuf.buf);
        HttpRequest req(SelectedClient.DataBuf.buf, client);
        int is_valid = req.parse();

        printf("\nsocket_id: %d\n", SelectedClient.Socket);

        if(is_valid != 0){
            printf("parsing error: %d\n", is_valid);
        }else{
            HttpResponse res(client);

            String key(req.getMethod());
            key.push(":");
            key.push(req.getPath());
            key.show();
            Server->handle(key, &req, &res);
            printf("\n");
            /*
            res.setHeader("Server", "MyServer");
            res.send("<h1>HelloWorld!</h1>");

             char default_response[] = "HTTP/1.1 200 OK\n\rContent-Length: 20\n\rServer: MyServer\n\rContent-Type: text/html\n\r\n\r<h1>HelloWorld!</h1>";

            // send_a_default_response
            int iSendResult = send(SelectedClient.Socket, default_response, 104, 0);
            //printf("Bytes sent: %d\n", iSendResult);
            closesocket(SelectedClient.Socket);
            */

            WSACloseEvent(EventArray[Index - WSA_WAIT_EVENT_0]);
            EventTotal -= 1;
        }
    }
}
