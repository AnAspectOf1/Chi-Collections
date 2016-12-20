#ifndef _CHI_FILE_H
#define _CHI_FILE_H

#include "int.h"
#include "io.h"
#include "stream.h"
#include "string.h"
#include <fcntl.h>


namespace chi {

	// A file that is opened from a path
	class File : public virtual SeekFileStream {};

	enum FileMode {
		FileMode_Append = O_APPEND,
		FileMode_Create = O_CREAT
	};

	class ReadFile : public virtual File, public ReadSeekFileStream {
	protected:
		ReadFile() {}
		ReadFile( int fd ) : FileStream( fd ) {}

	public:
		template <class T=void*>
		static ReadFile open( const StringBase& path, int mode = 0 ) {
			return ReadFile::open( path.ptr(), mode );
		}
		template <class T=void*>
		static ReadFile open( const char* path, int mode = 0 ) {
			int fd = ::open( path, O_RDONLY | mode );
			if ( fd == -1 )	throw UnknownIoException();
			return ReadFile( fd );
		}
	};

	class WriteFile : public virtual File, public WriteSeekFileStream {
	protected:
		WriteFile() {}
		WriteFile( int fd ) : FileStream( fd ) {}

	public:
		template <class T=void*>
		static WriteFile open( const StringBase& path, int mode = 0 ) {
			return WriteFile::open( path.ptr(), mode );
		}
		template <class T=void*>
		static WriteFile open( const char* path, int mode = 0 ) {
			int fd = ::open( path, O_WRONLY | mode );
			if ( fd == -1 )	throw UnknownIoException();
			return WriteFile( fd );
		}
	};

	class ReadWriteFile : public virtual ReadFile, public virtual WriteFile {
	protected:
		ReadWriteFile( int fd ) : FileStream( fd ) {}

	public:
		template <class T=void*>
		static WriteFile open( const StringBase& path, int mode = 0 ) {
			return WriteFile::open( path.ptr(), mode );
		}
		template <class T=void*>
		static ReadFile open( const char* path, int mode = 0 ) {
			int fd = ::open( path, O_RDWR | mode );
			if ( fd == -1 )	throw UnknownIoException();
			return ReadWriteFile( fd );
		}
	};
}

#endif//_CHI_FILE_H
