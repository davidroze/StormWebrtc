#include "StormWebrtc/StormWebrtc.h"
#include "StormWebrtcServerAPI/StormWebrtcServer.h"

int main()
{
  StormWebrtcStaticInit();

  static const int kMaxConnections = 256;

  StormWebrtcServerSettings settings;
  settings.m_MaxConnections = kMaxConnections;
  settings.m_Port = 61200;
  settings.m_KeyFile = "localhost.key";
  settings.m_CertFile = "localhost.crt";
  settings.m_InStreams.push_back(StormWebrtcStreamType::kReliable);
  settings.m_OutStreams.push_back(StormWebrtcStreamType::kReliable);

  auto server = CreateStormWebrtcServer(settings);

  StormWebrtcEvent ev;

  struct ClientData
  {
    bool m_HasClient = false;
    bool m_GotURL = false;
    StormWebrtcConnectionHandle m_ConnectionHandle;

    // Some time of http request handle here
  };

  ClientData clients[kMaxConnections];

  while (true)
  {
    server->Update();
    while (server->PollEvent(ev))
    {
      switch (ev.m_Type)
      {
      case StormWebrtcEventType::kConnected:
        clients[ev.m_ConnectionHandle.m_SlotId].m_HasClient = true;
        clients[ev.m_ConnectionHandle.m_SlotId].m_GotURL = false;
        clients[ev.m_ConnectionHandle.m_SlotId].m_ConnectionHandle =
ev.m_ConnectionHandle;
        break;
      case StormWebrtcEventType::kDisconnected:
        clients[ev.m_ConnectionHandle.m_SlotId].m_HasClient = false;

        // Cancel the http request if it's still going here

        break;
      case StormWebrtcEventType::kData:

        if(clients[ev.m_ConnectionHandle.m_SlotId].m_GotURL == false)
        {
          clients[ev.m_ConnectionHandle.m_SlotId].m_GotURL = true;

          // Convert the data passed in to a zero delimited string
          std::string url(ev.m_Buffer.get(), ev.m_Buffer.get() +
ev.m_DataSize);

          // Start the http request here using the url
        }
        break;
      }
    }

    // Check for http response completions here
  }

  StormWebrtcStaticCleanup();
  return 0;
}
