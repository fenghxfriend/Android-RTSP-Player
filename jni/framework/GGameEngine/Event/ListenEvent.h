#ifndef __LISTEN_EVENT__
#define __LISTEN_EVENT__

#include <vector>
#include <map>
#include <algorithm>
#include <memory>

template<class T>
class EventListener
{
public:
	EventListener()
	{
		mPriority = 0;
		mErase = false;
	}
	virtual ~EventListener()
	{

	}
	typedef EventListener< T > MyType;
	virtual void Notify( T ) =	0;

	void	SetErase( void ) { mErase = true; }
	bool	NeedErase( void ) const { return mErase; }

	inline bool operator<( char iPriority )		const
	{
		return mPriority < iPriority;
	}
	inline bool	operator<( const MyType& rhs )	const
	{
		return mPriority < rhs.mPriority;
	}

	inline bool	operator>( const MyType& rhs )	const
	{
		return mPriority > rhs.mPriority;
	}

	virtual void*	GetCode( void )
	{
		return ( this );
	}
	inline void	SetPriority( char iPriority )
	{
		mPriority = iPriority;
	}
	inline char GetPriority( void )	const
	{
		return mPriority;
	}

protected:
	bool mErase;
	char mPriority;
};

template<typename R, typename ParmClass>
class FunImpl
{
public:
	typedef R ResultType;
	typedef ParmClass Parm1;
	virtual R operator()(Parm1) = 0;
};


template<class Functor,class objC,class funC>
class MyFunImpl : public Functor::RealImpl
{
	typedef typename Functor::RealImpl Base;
public:

	typedef typename Base::ResultType ResultType;
	typedef typename Base::Parm1 Parm1;

	MyFunImpl(objC p ,funC memFn) : obj(p),func(memFn)
	{

	}
	ResultType operator()(Parm1 p1)
	{
		((*obj).*func)(p1);
	}
private:
	objC obj;
	funC func;
};

template<class R, class derivedClass>
class MyFun
{
public:
	typedef FunImpl<R,derivedClass> RealImpl;
	typedef R ResultType;
	typedef typename RealImpl::Parm1 Parm1;
	template <class PtrObj, typename MemFn>
	MyFun(const PtrObj& p, MemFn memFn)
	: mRealImpl(new MyFunImpl<MyFun,PtrObj,MemFn>(p, memFn))
	{}

	ResultType operator()(Parm1 p1) const
	{ 
		return (*mRealImpl)(p1); 
	}
private:
	std::auto_ptr<RealImpl> mRealImpl;
};

template<class T, class derivedClass>
class RealListener : public EventListener<T>
{
public:
	typedef MyFun<void,derivedClass> MyRealFun;

	template<class X,class funC> 
	RealListener(X* px, funC fun ) 
	: mFun(new MyRealFun(px,fun))
	{

	}

	virtual ~RealListener()
	{
        
	}

	virtual void Notify( T p1 )
	{
		(*mFun)(static_cast< derivedClass>(p1));
	}
private:
	std::auto_ptr<MyRealFun> mFun;
};

template<typename eventId,typename T>
class Listener
{
public:
	~Listener( void )	{ Clear();  mCurListenerCode = NULL;}

	typedef EventListener<T> CurEventListener;
	typedef std::vector< CurEventListener* > LIST_LISTENER;
	typedef	std::map< eventId, LIST_LISTENER >	MAP_LISTENER_LIST;
	typedef void* ListenerCode;

	struct			ListenerLessComparer
	{
		bool	operator()( const CurEventListener* lhs, int iId )		const
		{
			return	lhs ->GetPriority() < iId;
		}

		bool	operator() ( int iId, const CurEventListener* rhs )	const
		{
			return iId < rhs ->GetPriority();
		}

		bool	operator()( const CurEventListener* lhs, const CurEventListener* rhs )	const
		{
			return	lhs ->GetPriority() < rhs ->GetPriority();
		}
	};

	template< class X, class P>
	inline ListenerCode RegListerner(int iPriority, eventId id, X* px,void(X::*pf)( P ))
	{
		LIST_LISTENER& list = GetListeners(id);
		typedef RealListener<T,P> LISTENER_TYPE;
		LISTENER_TYPE* pListen = new LISTENER_TYPE(px,pf);
		pListen->SetPriority( iPriority );
        typename LIST_LISTENER::iterator lit = std::lower_bound( 
			list.begin(), list.end(), iPriority, ListenerLessComparer() );
		list.insert( lit, pListen );
		return ( pListen);
	}

	bool UnregListerner( eventId iEventId, ListenerCode iCode )
	{
		LIST_LISTENER& theList = GetListeners( iEventId );	
		typename LIST_LISTENER::iterator lit = theList.begin();

		for( ; lit != theList.end(); ++lit )
		{
			if ( reinterpret_cast<ListenerCode>(*lit) == iCode )
			{
				if (mCurListenerCode == iCode)
				{
					mCurListenerCode = NULL;
				}
				delete (*lit);
				theList.erase(lit);
				return true;
			}
		}
		return false;
	}

	bool FireEvent(eventId id, T pt)
	{
		typename MAP_LISTENER_LIST::iterator mit = mMapListenList.find( id );
		if ( mit == mMapListenList.end() )
		{
			return false;
		}

		LIST_LISTENER& kList = (*mit).second;
		for ( unsigned int i = 0 ; i < kList.size() ; )
		{
			CurEventListener* pkListener = kList[i];
			mCurListenerCode = reinterpret_cast<ListenerCode>(pkListener);

			pkListener->Notify(pt);
			if (mCurListenerCode != NULL)
			{
				++i;
			}
		}
		return true;
	}

	void			Clear( void )
	{
		typename MAP_LISTENER_LIST::iterator mit = mMapListenList.begin();
		CurEventListener* pkListener;
		for( ; mit != mMapListenList.end(); ++mit )
		{
			LIST_LISTENER& lis = mit ->second;
			typename LIST_LISTENER::iterator lit = lis.begin();
			for( ; lit != lis.end(); ++lit )
			{
				pkListener = (*lit);
				delete pkListener;
			}
		}
		mMapListenList.clear();
	}

protected:
	LIST_LISTENER&	GetListeners( eventId ieventId )
	{
		typename MAP_LISTENER_LIST::iterator mit = mMapListenList.find( ieventId );
		if ( mit == mMapListenList.end() )
		{
			mit = mMapListenList.insert( 
				typename MAP_LISTENER_LIST::value_type( ieventId, LIST_LISTENER())).first;
		}
		return (*mit).second;
	}

private:
	MAP_LISTENER_LIST mMapListenList;
	ListenerCode mCurListenerCode;
};

#endif

