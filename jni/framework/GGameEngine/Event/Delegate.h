#ifndef __DELEGATE__
#define __DELEGATE__

template<typename Func>
class SimpleEvent;

template<typename T1> 
class AbstractDelegate
{
public:
	virtual void Trigger(T1 p1) = 0;
};

template<typename R, typename T1>
class Delegate : public AbstractDelegate<T1>
{
public:
	typedef void (R::* Action)(T1);
	typedef void (R::* ConstAction)(T1) const;

public:
	union Rec{ R* receiver; const R* const_receiver; };
	union Act{ Action action; ConstAction const_action; };

public:
	Delegate(){ receiver.const_receiver = NULL; action.action = NULL; }
	Delegate(const R* rec, Action act){ receiver.const_receiver = rec; action.action = act; }
	Delegate(const R* rec, ConstAction act){ receiver.const_receiver = rec; action.const_action = act; }
	virtual void Trigger(T1 p1){ (receiver.receiver->*action.action)(p1); }

public:
	Rec receiver;
	Act action;
};

template<typename T1>
class SimpleEvent<void(T1)>
{
public:
	SimpleEvent() : delegate(NULL){}
	~SimpleEvent(){ Clear(); }

	template<typename R>
	void Connect(R* receiver, void (R::*action)(T1 p1))
	{
		if (NULL == delegate)
		{
			delegate = new Delegate<R,T1>(receiver, action);
		}
	}

	template<typename R>
	void Connect(const R* receiver, void (R::*action)(T1 p1) const)
	{
		if (NULL == delegate)
		{
			delegate = new Delegate<R,T1>(receiver, action);
		}
	}
	void Triggered(T1 p1)
	{
		if(delegate != NULL)
			delegate->Trigger(p1);
	}

	inline void operator ()(T1 p1)
	{
		Triggered(p1);
	}

	void Clear()
	{
		if(delegate != NULL)
		{
			delete delegate;
			delegate = NULL;
		}
	}
	bool IsDelegated()
	{
		if (delegate)
		{
			return true;
		}
		else
			return false;
	}
private:
	AbstractDelegate<T1>* delegate;
};


class AbstractDelegate1
{
public:
	virtual void Trigger() = 0;
};

template<typename R>
class Delegate1 : public AbstractDelegate1
{
public:
	typedef void (R::* Action)();
	typedef void (R::* ConstAction)() const;

public:
	union Rec{ R* receiver; const R* const_receiver; };
	union Act{ Action action; ConstAction const_action; };

public:
	Delegate1(){ receiver.const_receiver = NULL; action.action = NULL; }
	Delegate1(const R* rec, Action act){ receiver.const_receiver = rec; action.action = act; }
	Delegate1(const R* rec, ConstAction act){ receiver.const_receiver = rec; action.const_action = act; }
	virtual void Trigger(){ (receiver.receiver->*action.action)(); }

public:
	Rec receiver;
	Act action;
};

template<>
class SimpleEvent<void()>
{
public:
	SimpleEvent() : delegate(NULL){}
	~SimpleEvent(){ Clear(); }

	template<typename R>
	void Connect(R* receiver, void (R::*action)())
	{
		if (NULL == delegate)
		{
			delegate = new Delegate1<R>(receiver, action);
		}
	}

	template<typename R>
	void Connect(const R* receiver, void (R::*action)() const)
	{
		if (NULL == delegate)
		{
			delegate = new Delegate1<R>(receiver, action);
		}
	}
	void Triggered()
	{
		if(delegate != NULL)
			delegate->Trigger();
	}

	inline void operator ()()
	{
		Triggered();
	}

	void Clear()
	{
		if(delegate != NULL)
		{
			delete delegate;
			delegate = NULL;
		}
	}
	bool IsDelegated()
	{
		if (delegate)
		{
			return true;
		}
		else
			return false;
	}
private:
	AbstractDelegate1* delegate;
};

#endif
