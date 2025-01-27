/*
 * Copyright (c) 2006-2011 Christian Plattner. All rights reserved.
 * Please refer to the LICENSE.txt for licensing details.
 */
package ch.ethz.ssh2.transport;

import java.io.IOException;
import java.security.SecureRandom;

import ch.ethz.ssh2.ConnectionInfo;
import ch.ethz.ssh2.DHGexParameters;
import ch.ethz.ssh2.ServerHostKeyVerifier;
import ch.ethz.ssh2.crypto.CryptoWishList;
import ch.ethz.ssh2.crypto.KeyMaterial;
import ch.ethz.ssh2.crypto.cipher.BlockCipher;
import ch.ethz.ssh2.crypto.cipher.BlockCipherFactory;
import ch.ethz.ssh2.crypto.dh.DhExchange;
import ch.ethz.ssh2.crypto.dh.DhGroupExchange;
import ch.ethz.ssh2.crypto.digest.MAC;
import ch.ethz.ssh2.log.Logger;
import ch.ethz.ssh2.packets.PacketKexDHInit;
import ch.ethz.ssh2.packets.PacketKexDHReply;
import ch.ethz.ssh2.packets.PacketKexDhGexGroup;
import ch.ethz.ssh2.packets.PacketKexDhGexInit;
import ch.ethz.ssh2.packets.PacketKexDhGexReply;
import ch.ethz.ssh2.packets.PacketKexDhGexRequest;
import ch.ethz.ssh2.packets.PacketKexDhGexRequestOld;
import ch.ethz.ssh2.packets.PacketKexInit;
import ch.ethz.ssh2.packets.PacketNewKeys;
import ch.ethz.ssh2.packets.Packets;
import ch.ethz.ssh2.signature.DSAPublicKey;
import ch.ethz.ssh2.signature.DSASHA1Verify;
import ch.ethz.ssh2.signature.DSASignature;
import ch.ethz.ssh2.signature.RSAPublicKey;
import ch.ethz.ssh2.signature.RSASHA1Verify;
import ch.ethz.ssh2.signature.RSASignature;

/**
 * KexManager.
 * 
 * @author Christian Plattner
 * @version $Id: //DTV/MP_BR/DTV_X_IDTV0801_002298_3_001/android/kk-4.x/external/ganymed-ssh2/src/main/java/ch/ethz/ssh2/transport/KexManager.java#1 $
 */
public class KexManager
{
	private static final Logger log = Logger.getLogger(KexManager.class);

	KexState kxs;
	int kexCount = 0;
	KeyMaterial km;
	byte[] sessionId;
	ClientServerHello csh;

	final Object accessLock = new Object();
	ConnectionInfo lastConnInfo = null;

	boolean connectionClosed = false;

	boolean ignore_next_kex_packet = false;

	final TransportManager tm;

	CryptoWishList nextKEXcryptoWishList;
	DHGexParameters nextKEXdhgexParameters;

	ServerHostKeyVerifier verifier;
	final String hostname;
	final int port;
	final SecureRandom rnd;

	public KexManager(TransportManager tm, ClientServerHello csh, CryptoWishList initialCwl, String hostname, int port,
			ServerHostKeyVerifier keyVerifier, SecureRandom rnd)
	{
		this.tm = tm;
		this.csh = csh;
		this.nextKEXcryptoWishList = initialCwl;
		this.nextKEXdhgexParameters = new DHGexParameters();
		this.hostname = hostname;
		this.port = port;
		this.verifier = keyVerifier;
		this.rnd = rnd;
	}

	public ConnectionInfo getOrWaitForConnectionInfo(int minKexCount) throws IOException
	{
		boolean wasInterrupted = false;

		try
		{
			synchronized (accessLock)
			{
				while (true)
				{
					if ((lastConnInfo != null) && (lastConnInfo.keyExchangeCounter >= minKexCount))
						return lastConnInfo;

					if (connectionClosed)
						throw (IOException) new IOException("Key exchange was not finished, connection is closed.")
								.initCause(tm.getReasonClosedCause());

					try
					{
						accessLock.wait();
					}
					catch (InterruptedException e)
					{
						wasInterrupted = true;
					}
				}
			}
		}
		finally
		{
			if (wasInterrupted)
				Thread.currentThread().interrupt();
		}
	}

	private String getFirstMatch(String[] client, String[] server) throws NegotiateException
	{
		if (client == null || server == null)
			throw new IllegalArgumentException();

		if (client.length == 0)
			return null;

		for (int i = 0; i < client.length; i++)
		{
			for (int j = 0; j < server.length; j++)
			{
				if (client[i].equals(server[j]))
					return client[i];
			}
		}
		throw new NegotiateException();
	}

	private boolean compareFirstOfNameList(String[] a, String[] b)
	{
		if (a == null || b == null)
			throw new IllegalArgumentException();

		if ((a.length == 0) && (b.length == 0))
			return true;

		if ((a.length == 0) || (b.length == 0))
			return false;

		return (a[0].equals(b[0]));
	}

	private boolean isGuessOK(KexParameters cpar, KexParameters spar)
	{
		if (cpar == null || spar == null)
			throw new IllegalArgumentException();

		if (compareFirstOfNameList(cpar.kex_algorithms, spar.kex_algorithms) == false)
		{
			return false;
		}

		if (compareFirstOfNameList(cpar.server_host_key_algorithms, spar.server_host_key_algorithms) == false)
		{
			return false;
		}

		/*
		 * We do NOT check here if the other algorithms can be agreed on, this
		 * is just a check if kex_algorithms and server_host_key_algorithms were
		 * guessed right!
		 */

		return true;
	}

	private NegotiatedParameters mergeKexParameters(KexParameters client, KexParameters server)
	{
		NegotiatedParameters np = new NegotiatedParameters();

		try
		{
			np.kex_algo = getFirstMatch(client.kex_algorithms, server.kex_algorithms);

			log.info("kex_algo=" + np.kex_algo);

			np.server_host_key_algo = getFirstMatch(client.server_host_key_algorithms,
					server.server_host_key_algorithms);

			log.info("server_host_key_algo=" + np.server_host_key_algo);

			np.enc_algo_client_to_server = getFirstMatch(client.encryption_algorithms_client_to_server,
					server.encryption_algorithms_client_to_server);
			np.enc_algo_server_to_client = getFirstMatch(client.encryption_algorithms_server_to_client,
					server.encryption_algorithms_server_to_client);

			log.info("enc_algo_client_to_server=" + np.enc_algo_client_to_server);
			log.info("enc_algo_server_to_client=" + np.enc_algo_server_to_client);

			np.mac_algo_client_to_server = getFirstMatch(client.mac_algorithms_client_to_server,
					server.mac_algorithms_client_to_server);
			np.mac_algo_server_to_client = getFirstMatch(client.mac_algorithms_server_to_client,
					server.mac_algorithms_server_to_client);

			log.info("mac_algo_client_to_server=" + np.mac_algo_client_to_server);
			log.info("mac_algo_server_to_client=" + np.mac_algo_server_to_client);

			np.comp_algo_client_to_server = getFirstMatch(client.compression_algorithms_client_to_server,
					server.compression_algorithms_client_to_server);
			np.comp_algo_server_to_client = getFirstMatch(client.compression_algorithms_server_to_client,
					server.compression_algorithms_server_to_client);

			log.info("comp_algo_client_to_server=" + np.comp_algo_client_to_server);
			log.info("comp_algo_server_to_client=" + np.comp_algo_server_to_client);

		}
		catch (NegotiateException e)
		{
			return null;
		}

		try
		{
			np.lang_client_to_server = getFirstMatch(client.languages_client_to_server,
					server.languages_client_to_server);
		}
		catch (NegotiateException e1)
		{
			np.lang_client_to_server = null;
		}

		try
		{
			np.lang_server_to_client = getFirstMatch(client.languages_server_to_client,
					server.languages_server_to_client);
		}
		catch (NegotiateException e2)
		{
			np.lang_server_to_client = null;
		}

		if (isGuessOK(client, server))
			np.guessOK = true;

		return np;
	}

	public synchronized void initiateKEX(CryptoWishList cwl, DHGexParameters dhgex) throws IOException
	{
		nextKEXcryptoWishList = cwl;
		nextKEXdhgexParameters = dhgex;

		if (kxs == null)
		{
			kxs = new KexState();

			kxs.dhgexParameters = nextKEXdhgexParameters;
			PacketKexInit kp = new PacketKexInit(nextKEXcryptoWishList, rnd);
			kxs.localKEX = kp;
			tm.sendKexMessage(kp.getPayload());
		}
	}

	private boolean establishKeyMaterial()
	{
		try
		{
			int mac_cs_key_len = MAC.getKeyLen(kxs.np.mac_algo_client_to_server);
			int enc_cs_key_len = BlockCipherFactory.getKeySize(kxs.np.enc_algo_client_to_server);
			int enc_cs_block_len = BlockCipherFactory.getBlockSize(kxs.np.enc_algo_client_to_server);

			int mac_sc_key_len = MAC.getKeyLen(kxs.np.mac_algo_server_to_client);
			int enc_sc_key_len = BlockCipherFactory.getKeySize(kxs.np.enc_algo_server_to_client);
			int enc_sc_block_len = BlockCipherFactory.getBlockSize(kxs.np.enc_algo_server_to_client);

			km = KeyMaterial.create("SHA1", kxs.H, kxs.K, sessionId, enc_cs_key_len, enc_cs_block_len, mac_cs_key_len,
					enc_sc_key_len, enc_sc_block_len, mac_sc_key_len);
		}
		catch (IllegalArgumentException e)
		{
			return false;
		}
		return true;
	}

	private void finishKex() throws IOException
	{
		if (sessionId == null)
			sessionId = kxs.H;

		establishKeyMaterial();

		/* Tell the other side that we start using the new material */

		PacketNewKeys ign = new PacketNewKeys();
		tm.sendKexMessage(ign.getPayload());

		BlockCipher cbc;
		MAC mac;

		try
		{
			cbc = BlockCipherFactory.createCipher(kxs.np.enc_algo_client_to_server, true, km.enc_key_client_to_server,
					km.initial_iv_client_to_server);

			mac = new MAC(kxs.np.mac_algo_client_to_server, km.integrity_key_client_to_server);

		}
		catch (IllegalArgumentException e1)
		{
			throw new IOException("Fatal error during MAC startup!");
		}

		tm.changeSendCipher(cbc, mac);
		tm.kexFinished();
	}

	public static String[] getDefaultServerHostkeyAlgorithmList()
	{
		return new String[] { "ssh-rsa", "ssh-dss" };
	}

	public static void checkServerHostkeyAlgorithmsList(String[] algos)
	{
		for (int i = 0; i < algos.length; i++)
		{
			if (("ssh-rsa".equals(algos[i]) == false) && ("ssh-dss".equals(algos[i]) == false))
				throw new IllegalArgumentException("Unknown server host key algorithm '" + algos[i] + "'");
		}
	}

	public static String[] getDefaultKexAlgorithmList()
	{
		return new String[] { "diffie-hellman-group-exchange-sha1", "diffie-hellman-group14-sha1",
				"diffie-hellman-group1-sha1" };
	}

	public static void checkKexAlgorithmList(String[] algos)
	{
		for (int i = 0; i < algos.length; i++)
		{
			if ("diffie-hellman-group-exchange-sha1".equals(algos[i]))
				continue;

			if ("diffie-hellman-group14-sha1".equals(algos[i]))
				continue;

			if ("diffie-hellman-group1-sha1".equals(algos[i]))
				continue;

			throw new IllegalArgumentException("Unknown kex algorithm '" + algos[i] + "'");
		}
	}

	private boolean verifySignature(byte[] sig, byte[] hostkey) throws IOException
	{
		if (kxs.np.server_host_key_algo.equals("ssh-rsa"))
		{
			RSASignature rs = RSASHA1Verify.decodeSSHRSASignature(sig);
			RSAPublicKey rpk = RSASHA1Verify.decodeSSHRSAPublicKey(hostkey);

			log.debug("Verifying ssh-rsa signature");

			return RSASHA1Verify.verifySignature(kxs.H, rs, rpk);
		}

		if (kxs.np.server_host_key_algo.equals("ssh-dss"))
		{
			DSASignature ds = DSASHA1Verify.decodeSSHDSASignature(sig);
			DSAPublicKey dpk = DSASHA1Verify.decodeSSHDSAPublicKey(hostkey);

			log.debug("Verifying ssh-dss signature");

			return DSASHA1Verify.verifySignature(kxs.H, ds, dpk);
		}

		throw new IOException("Unknown server host key algorithm '" + kxs.np.server_host_key_algo + "'");
	}

	public synchronized void handleMessage(byte[] msg, int msglen) throws IOException
	{
		PacketKexInit kip;

		if (msg == null)
		{
			synchronized (accessLock)
			{
				connectionClosed = true;
				accessLock.notifyAll();
				return;
			}
		}

		if ((kxs == null) && (msg[0] != Packets.SSH_MSG_KEXINIT))
			throw new IOException("Unexpected KEX message (type " + msg[0] + ")");

		if (ignore_next_kex_packet)
		{
			ignore_next_kex_packet = false;
			return;
		}

		if (msg[0] == Packets.SSH_MSG_KEXINIT)
		{
			if ((kxs != null) && (kxs.state != 0))
				throw new IOException("Unexpected SSH_MSG_KEXINIT message during on-going kex exchange!");

			if (kxs == null)
			{
				/*
				 * Ah, OK, peer wants to do KEX. Let's be nice and play
				 * together.
				 */
				kxs = new KexState();
				kxs.dhgexParameters = nextKEXdhgexParameters;
				kip = new PacketKexInit(nextKEXcryptoWishList, rnd);
				kxs.localKEX = kip;
				tm.sendKexMessage(kip.getPayload());
			}

			kip = new PacketKexInit(msg, 0, msglen);
			kxs.remoteKEX = kip;

			kxs.np = mergeKexParameters(kxs.localKEX.getKexParameters(), kxs.remoteKEX.getKexParameters());

			if (kxs.np == null)
				throw new IOException("Cannot negotiate, proposals do not match.");

			if (kxs.remoteKEX.isFirst_kex_packet_follows() && (kxs.np.guessOK == false))
			{
				/*
				 * Guess was wrong, we need to ignore the next kex packet.
				 */

				ignore_next_kex_packet = true;
			}

			if (kxs.np.kex_algo.equals("diffie-hellman-group-exchange-sha1"))
			{
				if (kxs.dhgexParameters.getMin_group_len() == 0)
				{
					PacketKexDhGexRequestOld dhgexreq = new PacketKexDhGexRequestOld(kxs.dhgexParameters);
					tm.sendKexMessage(dhgexreq.getPayload());

				}
				else
				{
					PacketKexDhGexRequest dhgexreq = new PacketKexDhGexRequest(kxs.dhgexParameters);
					tm.sendKexMessage(dhgexreq.getPayload());
				}
				kxs.state = 1;
				return;
			}

			if (kxs.np.kex_algo.equals("diffie-hellman-group1-sha1")
					|| kxs.np.kex_algo.equals("diffie-hellman-group14-sha1"))
			{
				kxs.dhx = new DhExchange();

				if (kxs.np.kex_algo.equals("diffie-hellman-group1-sha1"))
					kxs.dhx.init(1, rnd);
				else
					kxs.dhx.init(14, rnd);

				PacketKexDHInit kp = new PacketKexDHInit(kxs.dhx.getE());
				tm.sendKexMessage(kp.getPayload());
				kxs.state = 1;
				return;
			}

			throw new IllegalStateException("Unkown KEX method!");
		}

		if (msg[0] == Packets.SSH_MSG_NEWKEYS)
		{
			if (km == null)
				throw new IOException("Peer sent SSH_MSG_NEWKEYS, but I have no key material ready!");

			BlockCipher cbc;
			MAC mac;

			try
			{
				cbc = BlockCipherFactory.createCipher(kxs.np.enc_algo_server_to_client, false,
						km.enc_key_server_to_client, km.initial_iv_server_to_client);

				mac = new MAC(kxs.np.mac_algo_server_to_client, km.integrity_key_server_to_client);

			}
			catch (IllegalArgumentException e1)
			{
				throw new IOException("Fatal error during MAC startup!");
			}

			tm.changeRecvCipher(cbc, mac);

			ConnectionInfo sci = new ConnectionInfo();

			kexCount++;

			sci.keyExchangeAlgorithm = kxs.np.kex_algo;
			sci.keyExchangeCounter = kexCount;
			sci.clientToServerCryptoAlgorithm = kxs.np.enc_algo_client_to_server;
			sci.serverToClientCryptoAlgorithm = kxs.np.enc_algo_server_to_client;
			sci.clientToServerMACAlgorithm = kxs.np.mac_algo_client_to_server;
			sci.serverToClientMACAlgorithm = kxs.np.mac_algo_server_to_client;
			sci.serverHostKeyAlgorithm = kxs.np.server_host_key_algo;
			sci.serverHostKey = kxs.hostkey;

			synchronized (accessLock)
			{
				lastConnInfo = sci;
				accessLock.notifyAll();
			}

			kxs = null;
			return;
		}

		if ((kxs == null) || (kxs.state == 0))
			throw new IOException("Unexpected Kex submessage!");

		if (kxs.np.kex_algo.equals("diffie-hellman-group-exchange-sha1"))
		{
			if (kxs.state == 1)
			{
				PacketKexDhGexGroup dhgexgrp = new PacketKexDhGexGroup(msg, 0, msglen);
				kxs.dhgx = new DhGroupExchange(dhgexgrp.getP(), dhgexgrp.getG());
				kxs.dhgx.init(rnd);
				PacketKexDhGexInit dhgexinit = new PacketKexDhGexInit(kxs.dhgx.getE());
				tm.sendKexMessage(dhgexinit.getPayload());
				kxs.state = 2;
				return;
			}

			if (kxs.state == 2)
			{
				PacketKexDhGexReply dhgexrpl = new PacketKexDhGexReply(msg, 0, msglen);

				kxs.hostkey = dhgexrpl.getHostKey();

				if (verifier != null)
				{
					boolean vres = false;

					try
					{
						vres = verifier.verifyServerHostKey(hostname, port, kxs.np.server_host_key_algo, kxs.hostkey);
					}
					catch (Exception e)
					{
						throw (IOException) new IOException(
								"The server hostkey was not accepted by the verifier callback.").initCause(e);
					}

					if (vres == false)
						throw new IOException("The server hostkey was not accepted by the verifier callback");
				}

				kxs.dhgx.setF(dhgexrpl.getF());

				try
				{
					kxs.H = kxs.dhgx.calculateH(csh.getClientString(), csh.getServerString(),
							kxs.localKEX.getPayload(), kxs.remoteKEX.getPayload(), dhgexrpl.getHostKey(),
							kxs.dhgexParameters);
				}
				catch (IllegalArgumentException e)
				{
					throw (IOException) new IOException("KEX error.").initCause(e);
				}

				boolean res = verifySignature(dhgexrpl.getSignature(), kxs.hostkey);

				if (res == false)
					throw new IOException("Hostkey signature sent by remote is wrong!");

				kxs.K = kxs.dhgx.getK();

				finishKex();
				kxs.state = -1;
				return;
			}

			throw new IllegalStateException("Illegal State in KEX Exchange!");
		}

		if (kxs.np.kex_algo.equals("diffie-hellman-group1-sha1")
				|| kxs.np.kex_algo.equals("diffie-hellman-group14-sha1"))
		{
			if (kxs.state == 1)
			{

				PacketKexDHReply dhr = new PacketKexDHReply(msg, 0, msglen);

				kxs.hostkey = dhr.getHostKey();

				if (verifier != null)
				{
					boolean vres = false;

					try
					{
						vres = verifier.verifyServerHostKey(hostname, port, kxs.np.server_host_key_algo, kxs.hostkey);
					}
					catch (Exception e)
					{
						throw (IOException) new IOException(
								"The server hostkey was not accepted by the verifier callback.").initCause(e);
					}

					if (vres == false)
						throw new IOException("The server hostkey was not accepted by the verifier callback");
				}

				kxs.dhx.setF(dhr.getF());

				try
				{
					kxs.H = kxs.dhx.calculateH(csh.getClientString(), csh.getServerString(), kxs.localKEX.getPayload(),
							kxs.remoteKEX.getPayload(), dhr.getHostKey());
				}
				catch (IllegalArgumentException e)
				{
					throw (IOException) new IOException("KEX error.").initCause(e);
				}

				boolean res = verifySignature(dhr.getSignature(), kxs.hostkey);

				if (res == false)
					throw new IOException("Hostkey signature sent by remote is wrong!");

				kxs.K = kxs.dhx.getK();

				finishKex();
				kxs.state = -1;
				return;
			}
		}

		throw new IllegalStateException("Unkown KEX method! (" + kxs.np.kex_algo + ")");
	}
}
