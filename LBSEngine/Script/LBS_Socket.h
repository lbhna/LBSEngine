#pragma once

// CLBS_Socket command target
class CLBS_Script_TCP;

class CLBS_Socket : public CSocket
{
protected:
	CLBS_Script_TCP*		m_pTCP;
public:
	CLBS_Socket(CLBS_Script_TCP* IN pTCP);
	virtual ~CLBS_Socket();
	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
};


