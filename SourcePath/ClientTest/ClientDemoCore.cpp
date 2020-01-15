

#include "ClientDemoCore.h"
#include "../PublicLib/Include/Common/tools.h"


ClientDemoCore::ClientDemoCore(): m_Sock(0), m_bInitial(false)
{}

ClientDemoCore::~ClientDemoCore()
{
	SAFE_RELEASE_SOCKET(m_Sock);
	WSACleanup();
}

bool ClientDemoCore::Initialize()
{
	m_bInitial = CreateServerLink("10.53.3.212:11111");
	return m_bInitial;
}

bool ClientDemoCore::Start()
{
	return true;
}

bool ClientDemoCore::MainLoop()
{
	if (!m_bInitial)
		return false;

	while (true)
	{
		SendMsg();
		ReciveMsg();
	}
	return true;
}

bool ClientDemoCore::CreateServerLink(const char* strServerAddr)
{
	//	初始化Sockect
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (!CheckStringValid(strServerAddr))
	{
		printf("Error Server Addr");
		return false;
	}

	std::vector<std::string> vServerAddr = SplitString(strServerAddr, ":");
	if (vServerAddr.size() != 2)
	{
		printf("Invalid Server Address[%s]", strServerAddr);
		return false;
	}

	SOCKADDR_IN serverAddrIn;
	ZeroMemory(&serverAddrIn, sizeof(SOCKADDR_IN));
	//	字符串转Socket地址(地址不能带有端口号，不然报错)
	inet_pton(AF_INET, vServerAddr[0].c_str(), &(serverAddrIn.sin_addr));
	int nValue = atoi(vServerAddr[1].c_str());
	unsigned short uPort = nValue;
	serverAddrIn.sin_port = htons(nValue);
	//	family一定要设置，不然就10047了
	serverAddrIn.sin_family = AF_INET;

	m_Sock = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == m_Sock)
	{
		printf("Create Socket Error");
		return false;
	}

	connect(m_Sock, (SOCKADDR*)&serverAddrIn, sizeof(SOCKADDR_IN));

	return INVALID_SOCKET != m_Sock;
}

bool ClientDemoCore::ReciveMsg()
{
	char recvBuff[1024] = { 0 };
	if (INVALID_SOCKET != m_Sock)
	{
		recv(m_Sock, recvBuff, 1024, 0);
	}

	if (CheckStringValid(recvBuff))
	{
		printf("Recive Server Msg[%s]", recvBuff);
	}

	return false;
}

bool ClientDemoCore::SendMsg()
{
	char sendBuff[1024] = { 0 };
	scanf("%s", &sendBuff);
	send(m_Sock, sendBuff, strlen(sendBuff) + 1, 0);
	return false;
}