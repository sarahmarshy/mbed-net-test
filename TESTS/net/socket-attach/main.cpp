#include <algorithm>
#include "mbed.h"
#include "net-help.h"
#include "TCPSocket.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest.h"

using namespace utest::v1;


namespace {
    // Test connection information
    const char *HTTP_SERVER_NAME = "developer.mbed.org";
    const char *HTTP_SERVER_FILE_PATH = "/media/uploads/mbed_official/hello.txt";
    const int HTTP_SERVER_PORT = 80;
#if defined(TARGET_VK_RZ_A1H)
    const int RECV_BUFFER_SIZE = 300;
#else
    const int RECV_BUFFER_SIZE = 512;
#endif
    // Test related data
    const char *HTTP_OK_STR = "200 OK";
    const char *HTTP_HELLO_STR = "Hello world!";

    // Test buffers
    char buffer[RECV_BUFFER_SIZE] = {0};

    Semaphore recvd;
    EventQueue queue;
}

bool find_substring(const char *first, const char *last, const char *s_first, const char *s_last) {
    const char *f = std::search(first, last, s_first, s_last);
    return (f != last);
}

void get_data(TCPSocket* sock){
    bool result = false;
    // Server will respond with HTTP GET's success code
    const int ret = sock->recv(buffer, sizeof(buffer) - 1);
    if(ret <= 0)
        return;

    buffer[ret] = '\0';

    // Find 200 OK HTTP status in reply
    bool found_200_ok = find_substring(buffer, buffer + ret, HTTP_OK_STR, HTTP_OK_STR + strlen(HTTP_OK_STR));
    // Find "Hello World!" string in reply
    bool found_hello = find_substring(buffer, buffer + ret, HTTP_HELLO_STR, HTTP_HELLO_STR + strlen(HTTP_HELLO_STR));

    TEST_ASSERT_TRUE(found_200_ok);
    TEST_ASSERT_TRUE(found_hello);

    if (found_200_ok && found_hello) result = true;

    TEST_ASSERT_EQUAL(result, true);

    printf("HTTP: Received %d chars from server\r\n", ret);
    printf("HTTP: Received 200 OK status ... %s\r\n", found_200_ok ? "[OK]" : "[FAIL]");
    printf("HTTP: Received '%s' status ... %s\r\n", HTTP_HELLO_STR, found_hello ? "[OK]" : "[FAIL]");
    printf("HTTP: Received message:\r\n");
    printf("%s", buffer);
    sock->close();
    // Signal that we have recvd
    recvd.release();
}

void test_socket_attach() {
    // Dispatch event queue
    Thread eventThread;
    eventThread.start(callback(&queue, &EventQueue::dispatch_forever));

    NetworkInterface* net = get_net();
    net_connect(net);
    printf("TCP client IP Address is %s\r\n", net->get_ip_address());

    TCPSocket sock(net);
    printf("HTTP: Connection to %s:%d\r\n", HTTP_SERVER_NAME, HTTP_SERVER_PORT);
    if (sock.connect(HTTP_SERVER_NAME, HTTP_SERVER_PORT) == 0) {
        printf("HTTP: OK\r\n");

        // We are constructing GET command like this:
        // GET http://developer.mbed.org/media/uploads/mbed_official/hello.txt HTTP/1.0\n\n
        strcpy(buffer, "GET http://");
        strcat(buffer, HTTP_SERVER_NAME);
        strcat(buffer, HTTP_SERVER_FILE_PATH);
        strcat(buffer, " HTTP/1.0\n\n");
        // Attach a sigio function that adds function to event queue
        sock.sigio(queue.event(get_data, &sock));
        // Send GET command
        sock.send(buffer, strlen(buffer));
        // wait for recv data
        recvd.wait();
    } else {
        printf("HTTP: ERROR\r\n");
    }
    net->disconnect();
}


// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(120, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Socket Attach Test", test_socket_attach),
};

Specification specification(test_setup, cases);

int main() {
    return !Harness::run(specification);
}