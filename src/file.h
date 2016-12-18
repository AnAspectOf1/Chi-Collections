#ifndef _CHI_FILE_H
#define _CHI_FILE_H

#include "int.h"
#include "io.h"
#include "stream.h"
#include "string.h"


namespace chi {

	// A file that is opened from a path
	class File : public virtual SeekFileStream {};

	enum FileMode {
		FileMode_Append = O_APPEND,
		FileMode_Create = O_CREAT
	};

	class ReadFile : public virtual File, public virtual ReadFileStream {
	protected:
		ReadFile( int fd ) : FileStream( fd ) {}

	public:
		template <class T=void*>
		static ReadFile open( const StringBase& path, int mode = 0 ) {
			int fd = open( filename, O_RDONLY | mode );
			if ( fd == -1 )	throw UnknownIoException();
			return ReadFile( fd );
		}
	};

	class WriteFile : public virtual File, public virtual WriteFileStream {
	protected:
		WriteFile( int fd ) : FileStream( fd ) {}

	public:
		template <class T=void*>
		static ReadFile open( const StringBase& path, int mode = 0 ) {
			int fd = open( filename, O_WRONLY | mode );
			if ( fd == -1 )	throw UnknownIoException();
			return WriteFile( fd );
		}
	};

	class ReadWriteFile : public ReadFile, public WriteFile {
	protected:
		ReadWriteFile( int fd ) : FileStream( fd ) {}

	public:
		template <class T=void*>
		static ReadFile open( const StringBase& path, int mode = 0 ) {
			int fd = open( filename, O_RDWR | mode );
			if ( fd == -1 )	throw UnknownIoException();
			return ReadWriteFile( fd );
		}
	};
}

#endif//_CHI_FILE_H
