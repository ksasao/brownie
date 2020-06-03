#ifndef ATOM_CLIENT_H_INCLUDE
#define ATOM_CLIENT_INCLUDE

class AtomClient {
  private:
  public:
    AtomClient();
    String getName();
    char* getClientId();
    void setup(String name, char* ssid, char* password, char* server);
    void reconnect(void);
    void publish(String topic, String body);
};

#endif
