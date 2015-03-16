#include <mtp/ptp/Session.h>
#include <mtp/ptp/Messages.h>
#include <mtp/ptp/Container.h>
#include <mtp/ptp/OperationRequest.h>

namespace mtp
{

	void Session::Close()
	{
		OperationRequest req(OperationCode::CloseSession, 0, _sessionId);
		Container container(req);
		_packeter.Write(container.Data);
		ByteArray data, response;
		_packeter.Read(data, response);
		//HexDump("payload", data);
	}

	msg::ObjectHandles Session::GetObjectHandles(u32 storageId, u32 objectFormat, u32 parent)
	{
		OperationRequest req(OperationCode::GetObjectHandles, _transactionId++, storageId, objectFormat, parent);
		Container container(req);
		_packeter.Write(container.Data);
		ByteArray data, response;
		_packeter.Read(data, response);
		InputStream stream(data, 8); //operation code + session id

		msg::ObjectHandles goh;
		goh.Read(stream);
		return goh;
	}
	msg::StorageIDs Session::GetStorageIDs()
	{
		OperationRequest req(OperationCode::GetStorageIDs, _transactionId++, 0xffffffffu);
		Container container(req);
		_packeter.Write(container.Data);
		ByteArray data, response;
		_packeter.Read(data, response);
		InputStream stream(data, 8); //operation code + session id

		msg::StorageIDs gsi;
		gsi.Read(stream);
		return gsi;
	}

	msg::StorageInfo Session::GetStorageInfo(u32 storageId)
	{
		OperationRequest req(OperationCode::GetStorageInfo, _transactionId++, storageId);
		Container container(req);
		_packeter.Write(container.Data);
		ByteArray data, response;
		_packeter.Read(data, response);
		InputStream stream(data, 8); //operation code + session id
		msg::StorageInfo gsi;
		gsi.Read(stream);
		return gsi;
	}

	msg::ObjectInfo Session::GetObjectInfo(u32 objectId)
	{
		OperationRequest req(OperationCode::GetObjectInfo, _transactionId++, objectId);
		Container container(req);
		_packeter.Write(container.Data);
		ByteArray data, response;
		_packeter.Read(data, response);
		InputStream stream(data, 8); //operation code + session id
		msg::ObjectInfo goi;
		goi.Read(stream);
		return goi;
	}

	msg::ObjectPropsSupported Session::GetObjectPropsSupported(u32 objectId)
	{
		OperationRequest req(OperationCode::GetObjectPropsSupported, _transactionId++, objectId);
		Container container(req);
		_packeter.Write(container.Data);
		ByteArray data, response;
		_packeter.Read(data, response);
		InputStream stream(data, 8); //operation code + session id
		msg::ObjectPropsSupported ops;
		ops.Read(stream);
		return ops;
	}

	ByteArray Session::GetObject(u32 objectId)
	{
		OperationRequest req(OperationCode::GetObject, _transactionId++, objectId);
		Container container(req);
		_packeter.Write(container.Data);
		ByteArray data, response;
		_packeter.Read(data, response);
		return ByteArray(data.begin() + 8, data.end());
	}

	Session::NewObjectInfo Session::SendObjectInfo(const msg::ObjectInfo &objectInfo, u32 storageId, u32 parentObject)
	{
		u32 transaction = _transactionId++;
		{
			OperationRequest req(OperationCode::SendObjectInfo, transaction, storageId, parentObject);
			Container container(req);
			_packeter.Write(container.Data);
		}
		{
			DataRequest req(OperationCode::SendObjectInfo, transaction);
			OutputStream stream(req.Data);
			objectInfo.Write(stream);
			Container container(req);
			_packeter.Write(container.Data);
		}
		ByteArray data, response;
		_packeter.Read(data, response);
		HexDump("response", response);
		InputStream stream(response, 8); //operation code + session id
		NewObjectInfo noi;
		stream >> noi.StorageId;
		stream >> noi.ParentObjectId;
		stream >> noi.ObjectId;
		return noi;
	}

	void Session::SendObject(const ByteArray &object)
	{
		u32 transaction = _transactionId++;
		{
			OperationRequest req(OperationCode::SendObject, transaction);
			Container container(req);
			_packeter.Write(container.Data);
		}
		{
			DataRequest req(OperationCode::SendObject, transaction);
			req.Append(object);
			Container container(req);
			_packeter.Write(container.Data, 0);
		}
		ByteArray data, response;
		_packeter.Read(data, response);
		HexDump("response", response);
	}


	void Session::DeleteObject(u32 objectId)
	{
		OperationRequest req(OperationCode::DeleteObject, _transactionId++, objectId);
		Container container(req);
		_packeter.Write(container.Data);
		ByteArray data, response;
		_packeter.Read(data, response);
	}

}