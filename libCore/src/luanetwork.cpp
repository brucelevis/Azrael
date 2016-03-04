#include "luanetwork.h"
#include "luaglobal.h"
#include "luasvr.h"
#include "app.h"
#include "zmq.h"
#include "mqnet.h"
#include "connection.h"



LUA_IMPLE(LuaNetwork, LuaNetwork);
LUA_METD(LuaNetwork)
LUA_METD_END

LUA_FUNC(LuaNetwork)
L_METHOD(LuaNetwork, ConnectRouter)
L_METHOD(LuaNetwork, ConnectDB)
L_METHOD(LuaNetwork, GetSessionIP)
L_METHOD(LuaNetwork, SetSessionUserData)
L_METHOD(LuaNetwork, CloseSession)
L_METHOD(LuaNetwork, SendPacket)
L_METHOD(LuaNetwork, SendToNet)
L_METHOD(LuaNetwork, SendToDB)
L_METHOD(LuaNetwork, SendToGameServer)
LUA_FUNC_END



int LuaNetwork::GetSessionIP(lua_State* L)
{
	int sn;
	Lua::argParse(L, "i", &sn);
	Net *pNet = ServerApp::get()->getNet();
	if (!pNet)
	{
		luaL_error(L, "Net have'nt been created!");
		return 0;
	}
	return Lua::returnValue(L, "s", pNet->sessionHostIP(sn));
}

int LuaNetwork::SetSessionUserData(lua_State* L)
{
	int sn;
	char *ud;
	Lua::argParse(L, "is", &sn, &ud);
	Net *pNet = ServerApp::get()->getNet();
	if (!pNet)
	{
		luaL_error(L, "Net have'nt been created!");
		return 0;
	}
	Session *s = pNet->getSession(sn);
	if (s)
		s->setUserData(ud);
	else
		luaL_error(L, "Can't find session %d", sn);
	return 0;
}

int LuaNetwork::CloseSession(lua_State* L)
{
	int sn;
	Lua::argParse(L, "i", &sn);
	Net *pNet = ServerApp::get()->getNet();
	if (!pNet)
	{
		luaL_error(L, "Net have'nt been created!");
		return 0;
	}
	Session *s = pNet->getSession(sn);
	if (s)
	{
		s->close();
		LOG("Close connection %d sn by script interface", sn);
	}
	return 0;
}

int LuaNetwork::ConnectRouter(lua_State* L)
{
	ServerApp::get()->connectRouter();
	return 0;
}

int LuaNetwork::ConnectDB(lua_State* L)
{
	ServerApp::get()->connectDB();
	return 0;
}

int LuaNetwork::SendPacket(lua_State* L)
{
	int target, sn, fid, t;
	int64 uid;

	Lua::argParse(L, "iiilt", &target, &fid, &sn, &uid, &t);
	Buf buf;
	int len = lua_objlen(L, t);
	for (int i = 1;i <= len; ++i)
	{
		lua_rawgeti(L, t, i);
		if(!lua_istable(L,-1))
		{
			luaL_error(L, "except table at index:%d", i);
		}
		resolvePacketTableItem(L, &buf);
		lua_pop(L, 1);
	}
	ServerApp::get()->SendPacket(target, fid, sn, uid, buf);
	return 0;
}

int LuaNetwork::SendToDB(lua_State* L)
{
	int channel, target, sn, fid, t;
	int64 uid;

	Lua::argParse(L, "iiiilt", &channel, &target, &fid, &sn, &uid, &t);
	Buf buf;
	int len = lua_objlen(L, t);
	for (int i = 1;i <= len; ++i)
	{
		lua_rawgeti(L, t, i);
		if(!lua_istable(L,-1))
		{
			luaL_error(L, "except table at index:%d", i);
		}
		resolvePacketTableItem(L, &buf);
		lua_pop(L, 1);
	}
	MQNet *mqnet = ServerApp::get()->getMQNet();
	if(mqnet)
	{
		mqnet->methodToDB(channel, target, fid, sn, uid, buf);
	}
	return 0;

}

int LuaNetwork::SendToNet(lua_State* L)
{
	int sn, fid, t;
	Lua::argParse(L, "iit", &fid, &sn, &t);

	Buf buf;
	int len = lua_objlen(L, t);
	for (int i = 1;i <= len; ++i)
	{
		lua_rawgeti(L, t, i);
		if(!lua_istable(L,-1))
		{
			luaL_error(L, "except table at index:%d", i);
		}
		resolvePacketTableItem(L, &buf);
		lua_pop(L, 1);
	}
	Net *net = ServerApp::get()->getNet();
	if (net)
	{
		net->sendPacket(sn, fid, buf);
	}
	return 0;
}

int LuaNetwork::SendToGameServer(lua_State* L)
{
	int target, sn, fid, t;
	int64 uid;
	
	Lua::argParse(L, "iiilt", &target, &fid, &sn, &uid, &t);
	
	Buf buf;
	//����ָ������������ֵ�ĳ��ȡ����� string ���Ǿ����ַ����ĳ��ȣ����� table ��
	//��ȡ���Ȳ����� ('#') �Ľ�������� userdata ������Ϊ�������ڴ��ĳߴ磻��������ֵ��Ϊ 0 ��
	int len = lua_objlen(L, t);
	for (int i = 1;i <= len; ++i)
	{
		//�� t[n] ��ֵѹջ������� t ��ָ�������� index ����һ��ֵ
		lua_rawgeti(L, t, i);
		if(!lua_istable(L,-1))
		{
			luaL_error(L, "except table at index:%d", i);
		}
		resolvePacketTableItem(L, &buf);
		lua_pop(L, 1);			//����ջ
	}
	//buf.dump();
	//ServerApp::get()->SendToGameServer(target, fid, sn, uid, buf);
	ServerApp::get()->SendPacket(target, fid, sn, uid, buf);

	//int i1 = buf.readInt();
	//int i11 = buf.readByte();
	//int i2 = buf.readShort();
	//int i4 = buf.readInt();
	//int64 i8 = buf.readInt64();
	//std::string s;
	//buf.readString(s);

	return 0;
}

void LuaNetwork::resolvePacketTableItem(lua_State* L, class Buf* buf)
{
	if (!buf)
	{
		return;
	}

	//lua_rawgeti(L, -1, -1);
	lua_rawgeti(L, -1, 1);
	int type = luaL_checkinteger(L, -1);
	lua_rawgeti(L, -2, 2);

	switch(type)
	{
	case 0:
		{
			size_t len = 0;
			const char *v = luaL_checklstring(L, -1, &len);
			buf->writeLitteString(v, len);
			break;
		}
	case 1:
		{
			int v = luaL_checkinteger(L, -1);
			buf->writeByte(v);
			break;
		}
	case 2:
		{
			int v = luaL_checkinteger(L, -1);
			buf->writeShort(v);
			break;
		}
	case 4:
		{
			int v = luaL_checkinteger(L, -1);
			buf->writeInt(v);
			break;
		}
	case 8:
		{
			int64 v = luaL_checknumber(L, -1);
			buf->writeInt64(v);
			break;
		}
	default:
		luaL_error(L, "undefine type:%d", type);
	}
	//�Ӷ�ջ�е��� n ��Ԫ��
	lua_pop(L, 2);
}



