#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include "MQTTClient.h"

int main(int argc, char **argv)
{
    char *address = "tcp://localhost:1883";
    char *client_id = "publish_client";
    char *topic = "mqtt_examples";
    char buf[1024];
    const int time_out = 10000;//ms
    int rv;
    int QOS = 1;
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message publish_msg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    conn_opts.keepAliveInterval = 60;
    conn_opts.cleansession = 1;

    MQTTClient_create(&client, address, client_id, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    if((rv = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
    printf("MQTTClient_connect failure:%s\n", strerror(errno));
        return 0;
    }
    publish_msg.qos = QOS;
    publish_msg.retained = 0;
	
    while(1)
    {
        printf("enter the message you want to send\n");
        fgets(buf, sizeof(buf), stdin);
        publish_msg.payload = (void *)buf;
        publish_msg.payloadlen = strlen(buf);
        MQTTClient_publishMessage(client, topic, &publish_msg, &token);
        printf("waiting for %d seconds for publication of %s on topic %s for client with CLIENTID :%s\n", time_out / 1000, buf, topic, client_id);
        rv = MQTTClient_waitForCompletion(client, token, time_out);
        printf("Message with delivery token %d delivered\n", rv);
        printf("%s\n", buf);//”√”⁄≤‚ ‘
        sleep(3);
    }
}









