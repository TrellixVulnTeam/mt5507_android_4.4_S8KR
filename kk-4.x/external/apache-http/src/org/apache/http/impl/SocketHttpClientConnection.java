/*
 * $HeadURL: http://svn.apache.org/repos/asf/httpcomponents/httpcore/trunk/module-main/src/main/java/org/apache/http/impl/SocketHttpClientConnection.java $
 * $Revision: #1 $
 * $Date: 2014/10/13 $
 *
 * ====================================================================
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 *
 */

package org.apache.http.impl;

import java.io.IOException;
import java.net.InetAddress;
import java.net.Socket;
import java.net.SocketException;

import org.apache.http.HttpInetConnection;
import org.apache.http.impl.io.SocketInputBuffer;
import org.apache.http.impl.io.SocketOutputBuffer;
import org.apache.http.io.SessionInputBuffer;
import org.apache.http.io.SessionOutputBuffer;
import org.apache.http.params.HttpConnectionParams;
import org.apache.http.params.HttpParams;

/**
 * Implementation of a client-side HTTP connection that can be bound to a 
 * network Socket in order to receive and transmit data.
 *
 * @author <a href="mailto:oleg at ural.ru">Oleg Kalnichevski</a>
 *
 * @version $Revision: #1 $
 * 
 * @since 4.0
 */
public class SocketHttpClientConnection 
        extends AbstractHttpClientConnection implements HttpInetConnection {

    private volatile boolean open;
    private Socket socket = null;
    
    public SocketHttpClientConnection() {
        super();
    }
    
    protected void assertNotOpen() {
        if (this.open) {
            throw new IllegalStateException("Connection is already open");
        }
    }
    
    protected void assertOpen() {
        if (!this.open) {
            throw new IllegalStateException("Connection is not open");
        }
    }

    protected SessionInputBuffer createSessionInputBuffer(
            final Socket socket, 
            int buffersize,
            final HttpParams params) throws IOException {
        return new SocketInputBuffer(socket, buffersize, params);
    }
    
    protected SessionOutputBuffer createSessionOutputBuffer(
            final Socket socket, 
            int buffersize,
            final HttpParams params) throws IOException {
        return new SocketOutputBuffer(socket, buffersize, params);
    }
    
    protected void bind(
            final Socket socket, 
            final HttpParams params) throws IOException {
        if (socket == null) {
            throw new IllegalArgumentException("Socket may not be null");
        }
        if (params == null) {
            throw new IllegalArgumentException("HTTP parameters may not be null");
        }
        this.socket = socket;

        int buffersize = HttpConnectionParams.getSocketBufferSize(params);

        init(
                createSessionInputBuffer(socket, buffersize, params), 
                createSessionOutputBuffer(socket, buffersize, params),
                params);
        
        this.open = true;
    }

    public boolean isOpen() {
        return this.open;
    }
    
    protected Socket getSocket() {
        return this.socket;
    }

    public InetAddress getLocalAddress() {
        if (this.socket != null) {
            return this.socket.getLocalAddress();
        } else {
            return null;
        }
    }

    public int getLocalPort() {
        if (this.socket != null) {
            return this.socket.getLocalPort();
        } else {
            return -1;
        }
    }

    public InetAddress getRemoteAddress() {
        if (this.socket != null) {
            return this.socket.getInetAddress();
        } else {
            return null;
        }
    }

    public int getRemotePort() {
        if (this.socket != null) {
            return this.socket.getPort();
        } else {
            return -1;
        }
    }

    public void setSocketTimeout(int timeout) {
        assertOpen();
        if (this.socket != null) {
            try {
                this.socket.setSoTimeout(timeout);
            } catch (SocketException ignore) {
                // It is not quite clear from the Sun's documentation if there are any 
                // other legitimate cases for a socket exception to be thrown when setting 
                // SO_TIMEOUT besides the socket being already closed
            }
        }
    }
    
    public int getSocketTimeout() {
        if (this.socket != null) {
            try {
                return this.socket.getSoTimeout();
            } catch (SocketException ignore) {
                return -1;
            }
        } else {
            return -1;
        }
    }

    public void shutdown() throws IOException {
        this.open = false;
        Socket tmpsocket = this.socket;
        if (tmpsocket != null) {
            tmpsocket.close();
        }
    }
    
    public void close() throws IOException {
        if (!this.open) {
            return;
        }
        this.open = false;
        doFlush();
        try {
            try {
                this.socket.shutdownOutput();
            } catch (IOException ignore) {
            }
            try {
                this.socket.shutdownInput();
            } catch (IOException ignore) {
            }
        } catch (UnsupportedOperationException ignore) {
            // if one isn't supported, the other one isn't either
        }
        this.socket.close();
    }
    
}
