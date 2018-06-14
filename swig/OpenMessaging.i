/* OpenMessaging */

%module OpenMessaging

%{
extern struct AccessPoint* createAccessPoint(const char* accessUrl);

extern struct Producer* createProducer(struct AccessPoint* accessPoint);

extern int startProducer(struct Producer* producer);

extern struct Message* createByteMessage(struct Producer*, const char* topic, signed char* body, unsigned int len);

extern struct SendResult* sendSync(struct Producer* producer, struct Message* message);

extern const char* msgId(struct SendResult* sendResult);

extern int shutdownProducer(struct Producer* producer);

extern void freeMessage(struct Message* message);

extern void freeProducer(struct Producer* producer);
%}

extern struct AccessPoint* createAccessPoint(const char* accessUrl);

extern struct Producer* createProducer(struct AccessPoint* accessPoint);

extern int startProducer(struct Producer* producer);

extern struct Message* createByteMessage(struct Producer*, const char* topic, signed char* body, unsigned int len);

extern struct SendResult* sendSync(struct Producer* producer, struct Message* message);

extern const char* msgId(struct SendResult* sendResult);

extern int shutdownProducer(struct Producer* producer);

extern void freeMessage(struct Message* message);

extern void freeProducer(struct Producer* producer);