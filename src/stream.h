#ifndef _CHI_STREAM_H
#define _CHI_STREAM_H

#include "exception.h"
#include <chi/buffer.h>
#include <chi/dynamic.h>
#include <chi/linked.h>
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

		virtual Byte readByte() {
			Buffer<> buffer(1);
			this->read( buffer );
			return buffer[0];
		}

		Buffer<> read( Size length ) {
			Buffer<> buffer( length );
			this->read( buffer );
			return buffer;
		}

		char readChar() {
			char test = this->readByte();
			return test;
		}

		String<> readLine() {
			//LinkedList<char> buffer;
			DynamicString line;

			try {
			char c = this->readChar();
				while ( (c = this->readChar()) != '\n' ) {
					line += c;
				}
			}
			catch ( Exception& e ) {
				printf("caught.....\n");
			}

			line += '\n';
			printf( "test......\n" );
			return line;
		}

		chi::String<> readString( chi::Size length ) {
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
		virtual Size write( const Buffer<>& buffer ) = 0;

		virtual bool writeByte( Byte byte ) {
			return this->write( Buffer<>( 1, byte ) ) == 1;
		}

		Size writeString( const ArrayBase<char>& string ) {
			Buffer<> buffer( string.count() - 1 );

			for ( Size i = 0; i < buffer.count(); i++ ) {
				buffer[i] = string[i];
			}

			return this->write( buffer );
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

	class ReadSeekStream : public virtual ReadStream, public virtual Seekable {};
	class WriteSeekStream : public virtual WriteStream, public virtual Seekable {};
	class ReadWriteSeekStream : public virtual ReadSeekStream, public virtual WriteSeekStream {};

	class BufferedReadStream : public ReadSeekStream {
		ReadStream* stream;
		Buffer<FutureAllocator<Byte>> buffer;	// TODO: Transform this into some kind of linked list of buffers, for better speed.
		Size pos;

	public:
		BufferedReadStream( ReadStream* stream ) : stream(stream), pos(0) {}

		void close()	{ return this->stream->close(); }

		void move( SSize position ) {
			this->seek( this->pos + position );
		}

		Size position() const	{ return this->pos; }

		void read( BufferBase& buffer ) {
			if ( this->pos + buffer.count() < this->buffer.count() )
				buffer = this->buffer.slice( this->pos, buffer.count() );

			else {
				// Fill up buffer with what we already know
				Size i = 0;
				for ( ; this->pos < this->buffer.count(); i++, this->pos++ ) {
					buffer[i] = this->buffer[ this->pos ];

				}

				// Then read the rest from the stream
				BufferSlice slice = buffer.slice( i, buffer.count() - i );
				this->stream->read( slice );

				// And add it to our buffer
				this->buffer += slice;
			}
		}

		Byte readByte()	{
			Byte byte;			

			if ( this->pos < this->buffer.count() )
				byte = this->buffer[ this->pos ];
			else {
				byte = this->stream->readByte();
				this->buffer += byte;
			}

			this->pos++;
			return byte;
		}

		void seek( Size position ) {
			CHI_ASSERT( position >= this->buffer.count(), "Can't seek beyond what is already read" );
			
			this->pos = position;
		}

	};
}

#endif//_CHI_STREAM_H
