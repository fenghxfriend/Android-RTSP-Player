#include "GEventQueue.h"

#define GEventQueueDefaultCapacity  256 // 256个指令

GEventQueue::GEventQueue (			    ) { init   ( GEventQueueDefaultCapacity ); }
GEventQueue::GEventQueue ( int capacity ) { init   ( capacity 					); }
GEventQueue::~GEventQueue(			    ) { destory(							); }

void GEventQueue::init(int capacity)
{
	//检查参数
	if ( capacity <= 0 ) capacity = GEventQueueDefaultCapacity;

	//初始化全局变量
	mDataBytes 			= 0;
	mDataPtrOffset		= 0;
	mBufferCapacity		= capacity;
	mBufferExpandMargin	= capacity;
	mLock 				= mUtility.createLock();

	//创建缓冲区
	mBufferPtr = ( char * )malloc( mBufferCapacity * sizeof( GEventData ) );
}

void GEventQueue::destory()
{
	mUtility.lock(mLock);
	free( mBufferPtr );
	mDataBytes 			= 0;
	mDataPtrOffset		= 0;
	mBufferPtr			= 0;
	mBufferCapacity		= 0;
	mBufferExpandMargin	= 0;
	mUtility.unlock(mLock);
	mUtility.destorylock( &mLock );
}

void GEventQueue::clear()
{
	mUtility.lock(mLock);
	mDataBytes 		= 0;
	mDataPtrOffset	= 0;
	mUtility.unlock(mLock);
}

void GEventQueue::write(char *data, int size)
{
	do
	{
		//判断参数是否有效
		if ( !data || size <= 0 ) break;

		//写入数据
		write0( data, size, true );

	} while(0);
}

int GEventQueue::read(char *buffer, int maxSize)
{
	int result = 0;

	do
	{
		//判断参数是否有效
		if ( !buffer || maxSize <= 0 ) break;

		//加锁
		mUtility.lock(mLock);

		//读取数据
		do
		{
			//判断当前缓冲区中数据量
			if ( mDataBytes <= 0 ) break;

			//解析本次读取数据量
			int readSize = ( mDataBytes >= maxSize ? maxSize : mDataBytes );

			//判断线性数据是否够
			int lineSize = mBufferCapacity - mDataPtrOffset;
			if ( lineSize >= readSize )
			{
				memcpy( buffer, mBufferPtr + mDataPtrOffset, readSize );
			}
			else
			{
				memcpy( buffer			 , mBufferPtr + mDataPtrOffset, lineSize 		    );
				memcpy( buffer + lineSize, mBufferPtr				  , readSize - lineSize );
			}

			//设定flag
			mDataPtrOffset 	+= readSize;
			mDataBytes		-= readSize;
			mDataPtrOffset  %= mBufferCapacity;
			result			= readSize;

		} while(0);

		//解锁
		mUtility.unlock(mLock);

	} while(0);

	return result;
}

void GEventQueue::write0(char *data, int size, bool lock)
{
	//加锁
	if ( lock ) mUtility.lock(mLock);

	//判断缓冲区剩余容量是否够用
	if ( size <= ( mBufferCapacity - mDataBytes ) )
	{
		//取得当前数据结束位置偏移量
		int dataEndOffset = mDataPtrOffset + mDataBytes;
		dataEndOffset %= mBufferCapacity;

		//判断线性容量是足够
		int lineSize = mBufferCapacity - dataEndOffset;
		if ( lineSize >= size )
		{
			//拷贝数据
			memcpy( mBufferPtr + dataEndOffset, data, size );
		}
		else
		{
			//拷贝数据
			memcpy( mBufferPtr + dataEndOffset, data		   , lineSize 		 );
			memcpy( mBufferPtr				  , data + lineSize, size - lineSize );
		}

		//累计数据量
		mDataBytes += size;
	}
	else
	{
		//扩充容量
		expandCapacity();

		//写入数据
		write0( data, size, false );
	}

	//解锁
	if ( lock ) mUtility.unlock(mLock);
}

void GEventQueue::expandCapacity()
{
	//重新申请buffer
	char *newBuffer = ( char * )malloc( ( mBufferCapacity + mBufferExpandMargin ) * sizeof( GEventData ) );

	//拷贝旧buffer数据至新buffer
	if ( ( mDataPtrOffset + mDataBytes ) <= mBufferCapacity ) //当前数据线性存放
	{
		memcpy( newBuffer, mBufferPtr + mDataPtrOffset, mDataBytes );
	}
	else
	{
		int lineSize = mBufferCapacity - mDataPtrOffset;
		memcpy( newBuffer			, mBufferPtr + mDataPtrOffset, lineSize 			 );
		memcpy( newBuffer + lineSize, mBufferPtr				 , mDataBytes - lineSize );
	}

	//释放旧内存
	free( mBufferPtr );

	//初始化全局变量
	mDataPtrOffset 	=  0;
	mBufferPtr		=  newBuffer;
	mBufferCapacity	+= mBufferExpandMargin;
}
