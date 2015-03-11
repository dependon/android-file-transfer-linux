#ifndef INTERFACE_H
#define	INTERFACE_H

#include <mtp/types.h>
#include <libusb.h>

namespace mtp { namespace usb
{
	enum struct EndpointType
	{
		Control = 0, Isochronous = 1, Bulk = 2, Interrupt = 3
	};

	enum struct EndpointDirection
	{
		In, Out
	};


	class Endpoint
	{
		const libusb_endpoint_descriptor & _endpoint;

	public:
		Endpoint(const libusb_endpoint_descriptor & endpoint) : _endpoint(endpoint) { }

		u8 GetAddress() const
		{ return _endpoint.bEndpointAddress; }

		int GetMaxPacketSize() const
		{ return _endpoint.wMaxPacketSize; }

		EndpointDirection GetDirection() const
		{
			u8 dir = GetAddress() & LIBUSB_ENDPOINT_DIR_MASK;
			if (dir == LIBUSB_ENDPOINT_IN)
				return EndpointDirection::In;
			else
				return EndpointDirection::Out;
		}

		EndpointType GetType() const
		{
			return EndpointType(_endpoint.bmAttributes & 3);
		}
	};
	DECLARE_PTR(Endpoint);

	class Configuration;
	DECLARE_PTR(Configuration);

	class Interface
	{
		ConfigurationPtr					_config;
		const libusb_interface_descriptor &	_interface;

	public:
		Interface(ConfigurationPtr config, const libusb_interface_descriptor &interface): _config(config), _interface(interface) { }

		int GetIndex() const
		{ return _interface.bInterfaceNumber; }

		int GetNameIndex() const
		{ return _interface.iInterface; }

		EndpointPtr GetEndpoint(int idx) const
		{ return std::make_shared<Endpoint>(_interface.endpoint[idx]); }

		int GetEndpointsCount() const
		{ return _interface.bNumEndpoints; }
	};
	DECLARE_PTR(Interface);

}}

#endif
