/* Generated By:JavaCC: Do not edit this line. StringProvider.java Version 6.1 */
/* JavaCCOptions:KEEP_LINE_COLUMN=true */
/*
 *
 * This file is part of Java 1.8 parser and Abstract Syntax Tree.
 *
 * Java 1.8 parser and Abstract Syntax Tree is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Java 1.8 parser and Abstract Syntax Tree.  If not, see <http://www.gnu.org/licenses/>.
 */
package com.github.javaparser;

	
	import java.io.IOException;
	
	public class StringProvider implements Provider {

		String _string;
		int _position = 0;
		int _size;
		
		public StringProvider(String string) {
			_string = string;
			_size = string.length();
		}
		
		@Override
		public int read(char[] cbuf, int off, int len) throws IOException {
			int numCharsOutstandingInString = _size - _position;
			
			if (numCharsOutstandingInString == 0) {
				return -1;
			}
			
			int numBytesInBuffer = cbuf.length;
			int numBytesToRead = numBytesInBuffer -off;
			numBytesToRead = numBytesToRead > len ? len : numBytesToRead;
			
			if (numBytesToRead > numCharsOutstandingInString) {
				numBytesToRead = numCharsOutstandingInString;
			}
			
			_string.getChars(_position, _position + numBytesToRead, cbuf, off);
			
			_position += numBytesToRead;
			
			return numBytesToRead;
		}

		@Override
		public void close() throws IOException {
			_string = null;
		}
		
	}
/* JavaCC - OriginalChecksum=d4625218ed283e295a6dcd153d6a202a (do not edit this line) */
