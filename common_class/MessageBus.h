#pragma once

#include"Any.h"
#include<map>
#include<functional>
#include<typeinfo>
#include<assert.h>
using namespace std;

template <typename R = void>
class MessageBus //: boost::noncopyable
{
public:
	//注册消息
	template< class... Args, class F, class = typename std::enable_if<!std::is_member_function_pointer<F>::value>::type>
	void Attach(string strKey, F && f)
	{
		assert(m_map.find(strKey)==m_map.end());
		std::function<R(Args...)> fn = [&](Args... args) {return f(std::forward<Args>(args)...); };
		m_map.insert(std::make_pair(strKey, std::move(fn)));
	}

	// non-const member function 
	template<class... Args, class C, class... DArgs, class P>
	void Attach(string strKey, R(C::*f)(DArgs...), P && p)
	{
		assert(m_map.find(strKey) == m_map.end());
			//_ASSERTE("have same str Message");
		std::function<R(Args...)> fn = [&, f](Args... args) {return (*p.*f)(std::forward<Args>(args)...); };
		m_map.insert(std::make_pair(strKey, std::move(fn)));
	}

	template<class... Args, class C, class... DArgs, class P>
	void Attach(string strKey, R(C::*f)(DArgs...) const, P && p)
	{
		assert(m_map.find(strKey) == m_map.end());
		std::function<R(Args...)> fn = [&, f](Args... args) {return (*p.*f)(std::forward<Args>(args)...); };
		m_map.insert(std::make_pair(strKey, std::move(fn)));
	}

	//广播消息，主题和参数可以确定一个消息, 所有的消息接收者都将收到并处理该消息
	template<typename... Args>
	void SendReq(string strTopic, Args... args)
	{
		auto range = m_map.equal_range(strTopic);
		for (auto it = range.first; it != range.second; it++)
		{
			std::function<R(Args...)> f = (it->second).AnyCast<std::function<R(Args...)>>();
			f(args...);
		}
	}
	
	//移除消息
	template<typename... Args>
	void Remove(string strTopic)
	{
		string strMsgType = GetNameofMsgType<Args...>();
		auto range = m_map.equal_range(strTopic + strMsgType);
		m_map.erase(range.first, range.second);
	}

private:
	std::multimap<string, Any> m_map;
};