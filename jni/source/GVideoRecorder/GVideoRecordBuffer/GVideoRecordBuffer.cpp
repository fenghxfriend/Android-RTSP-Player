#include "GVideoRecordBuffer.h"

#define GVideoRecordBufferDefaultCapacity  ( 50 * 1024 ) // 50KB

GVideoRecordBuffer::GVideoRecordBuffer (			  ) { init   ( GVideoRecordBufferDefaultCapacity ); }
GVideoRecordBuffer::GVideoRecordBuffer ( int capacity ) { init   ( capacity 					   	 ); }
GVideoRecordBuffer::~GVideoRecordBuffer(			  ) { destory(								   	 ); }

void GVideoRecordBuffer::init(int capacity)
{
	//检查参数
	if ( capacity <= 0 ) capacity = GVideoRecordBufferDefaultCapacity;

	//初始化全局变量
	mDataBytes 			= 0;
	mDataPtrOffset		= 0;
	mIntTypeLength		= sizeof( int );
	mBufferCapacity		= capacity;
	mBufferExpandMargin	= capacity;
	mLock 				= mUtility.createLock();

	//创建缓冲区
	mBufferPtr = ( char * )malloc( mBufferCapacity );
}

void GVideoRecordBuffer::destory()
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

void GVideoRecordBuffer::clear()
{
	mUtility.lock(mLock);
	mDataBytes 		= 0;
	mDataPtrOffset	= 0;
	mUtility.unlock(mLock);
}

void GVideoRecordBuffer::writePacket(char *data, int size)
{
	do
	{
		//判断参数是否有效
		if ( !data || size <= 0 ) break;

		//写入数据
		write0( ( char * )( &size )	, mIntTypeLength, true , false );
		write0( data 				, size			, false, true  );

	} while(0);
}

int GVideoRecordBuffer::readPacket(char *buffer)
{
	int result = 0;

	if ( read0( ( char * )( &result ), mIntTypeLength, true, false ) > 0 )
	{
		result = read0( buffer, result, false, true );
	}
	else
	{
		//解锁
		mUtility.unlock(mLock);

		result = 0;
	}

	return result;
}

int GVideoRecordBuffer::read0(char *buffer, int size, bool beginLock, bool endUnLock)
{
	int result = 0;

	do
	{
		//判断参数是否有效
		if ( !buffer || size <= 0 ) break;

		//加锁
		if ( beginLock ) mUtility.lock(mLock);

		//读取数据
		do
		{
			//判断当前缓冲区中数据量
			if ( mDataBytes <= 0 ) break;

			//解析本次读取数据量
			int readSize = ( mDataBytes >= size ? size : mDataBytes );

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
		if ( endUnLock ) mUtility.unlock(mLock);

	} while(0);

	return result;
}

void GVideoRecordBuffer::write0(char *data, int size, bool beginLock, bool endUnLock)
{
	//加锁
	if ( beginLock ) mUtility.lock(mLock);

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
		write0( data, size, false, false );
	}

	//解锁
	if ( endUnLock ) mUtility.unlock(mLock);
}

void GVideoRecordBuffer::expandCapacity()
{
	//重新申请buffer
	char *newBuffer = ( char * )malloc( mBufferCapacity + mBufferExpandMargin );

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
