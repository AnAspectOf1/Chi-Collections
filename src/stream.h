#ifndef _CHI_STREAM_H
#define _CHI_STREAM_H

#include "exception.h"
#include <chi/buffer.h>
#include <chi/string.h>


namespace chi {

	class StreamException : public Exception {};
	class EndOfStreamException : public StreamException {};

	class Stream {
	public:
		virtual void close() = 0;
	};

	class Readable {
	public:
		virtual void read( BufferBase& buffer ) = 0;

		virtual chi::Byte readByte() {
			chi::Buffer<> buffer(1);
			this->read( buffer );
			return buffer[0];
		}

		Buffer<> read( Size length ) {
			Buffer<> buffer( length );
			this->read( buffer );
			return buffer;
		}

		char readChar() {
			return this->readByte();
		}

		virtual chi::String<> readString( chi::Size length ) {
			chi::String<> string( length + 1 );
			chi::Buffer<> buffer = this->read( length );
			
			for ( chi::Size i = 0; i < buffer.count(); i++ ) {
				string[i] = buffer[i];
			}
			string[ buffer.count() ] = '\0';

			return string;
		}
	};

	class ReadStream : public virtual Stream, public Readable {};

	class Writable {
	public:
		virtual chi::Size write( const chi::Buffer<>& buffer ) = 0;

		virtual bool writeByte( chi::Byte byte ) {
			return this->write( chi::Buffer<>( 1, byte ) ) == 1;
		}
	};

	class WriteStream : public virtual Stream, public Writable {};

	class Seekable {
	public:
		// Changes the current position absolutely
		virtual void seek( chi::Size position ) = 0;
		virtual chi::Size position() const = 0;
		
		// Changes the current position relatively
		virtual void move( chi::SSize steps )	{ this->seek( (chi::SSize)this->position() + steps ); }
	};

	class BufferedReadStream : public virtual ReadStream, public Seekable {
		ReadStream& stream;
		Buffer<FutureAllocator<Byte>> buffer;	// TODO: Transform this into some kind of linked list of buffers
		Size block_size;
		Size position;

	public:
		BufferedReadStream( ReadStream& stream, Size block_size = 1024 ) : stream(stream), block_size(block_size), position(0) {}

		Buffer<> read( chi::Size length ) {
			Buffer<> buffer;

			if ( this->position + length < this->buffer.count() )
				return this->buffer.slice( this->position, length );

			// Else
			// File up buffer with what we already know
			Size i = 0;
			for ( ; this->position < this->buffer.count(); i++, this->position++ ) {
				buffer[i] = this->buffer[ this->position ];

			}

			// Then read the rest from the stream
			ArraySlice<Byte> slice = buffer.slice( i, buffer.count() - i );
			this->stream.read( slice );

			return buffer;
		}

		Byte readByte()	{
			Byte byte;			

			if ( this->position < this->buffer.count() )
				byte = this->buffer[ this->position ];
			else {
				byte = this->stream.readByte();
				this->buffer += byte;
			}

			this->position++;
			return byte;
		}
	};
}

#endif//_CHI_STREAM_H
