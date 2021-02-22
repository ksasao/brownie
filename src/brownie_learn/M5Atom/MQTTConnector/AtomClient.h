#ifndef __ATOM_CLIENT_H__
#define __ATOM_CLIENT_H__

class AtomClient {
  private:
  public:
    AtomClient();
    String getName();
    char* getClientId();
    void setup(String name, char* ssid, char* password, char* server);
    void reconnect(void);
    void publish(String topic, String body);
    void reboot();
};

#endif
