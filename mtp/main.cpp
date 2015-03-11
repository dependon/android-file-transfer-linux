#include <stdio.h>
#include <sstream>
#include <stdexcept>

#include <mtp/usb/BulkPipe.h>
#include <mtp/usb/Context.h>
#include <mtp/usb/call.h>
#include <mtp/ptp/OperationRequest.h>
#include <mtp/ptp/Container.h>
#include <mtp/ptp/Messages.h>

int main(int argc, char **argv)
{
	using namespace mtp;
	usb::Context ctx;
	usb::DeviceDescriptorPtr desc;
	for (usb::DeviceDescriptorPtr dd : ctx.GetDevices())
	{
		if (dd->GetVendorId() == 0x18d1)
		{
			desc = dd;
			break;
		}
	}
	if (!desc)
		throw std::runtime_error("no mtp device found");

	usb::DevicePtr device = desc->Open();
	int confs = desc->GetConfigurationsCount();
	printf("configurations: %d\n", confs);

	int mtp_configuration = -1;
	int mtp_interface = -1;

	usb::ConfigurationPtr		configuration;
	usb::InterfacePtr			interface;

	for(int i = 0; i < confs; ++i)
	{
		usb::ConfigurationPtr conf = desc->GetConfiguration(i);
		int interfaces = conf->GetInterfaceCount();
		printf("interfaces: %d\n", interfaces);
		for(int j = 0; j < interfaces; ++j)
		{
			usb::InterfacePtr iface = conf->GetInterface(conf, j, 0);
			printf("%d:%d index %u, eps %u\n", i, j, iface->GetIndex(), iface->GetEndpointsCount());
			int name_idx = iface->GetNameIndex();
			if (!name_idx)
				continue;
			std::string name = device->GetString(name_idx);
			if (name == "MTP")
			{
				configuration = conf;
				interface = iface;
				mtp_configuration = i;
				mtp_interface = j;
				i = confs;
				break;
			}
		}
	}

	if (!interface || mtp_interface < 0 || mtp_configuration < 0)
		throw std::runtime_error("no mtp interface found");

	//device->SetConfiguration(configuration->GetIndex());
	usb::BulkPipePtr pipe = usb::BulkPipe::Create(device, interface);
	//USB_CALL(libusb_reset_device(device->GetHandle()));

	printf("claiming interface %d...\n", interface->GetIndex());
	//USB_CALL(libusb_claim_interface(device->GetHandle(), interface->GetIndex()));
	printf("claimed interface\n");
	//USB_CALL(libusb_set_interface_alt_setting(device->GetHandle(), mtp_interface, 0));

	//OperationRequest req(OperationCode::OpenSession, 0, 1);
	OperationRequest req(OperationCode::GetDeviceInfo, 0, 0);
	Container container(req);

	/*
	for(size_t i = 0; i < container.Data.size(); ++i)
		printf("%02x ", container.Data[i]);
	printf("\n");
	*/

#if 0
	std::vector<u8> data(in->GetMaxPacketSize());
	//std::vector<u8> data(64);

	printf("bulk transfer start, endpoint: %02x\n", out->GetAddress());
	int tr = 0;
	int r = libusb_bulk_transfer(device->GetHandle(), out->GetAddress(), container.Data.data(), container.Data.size(), &tr, 1000);
	printf("bulk transfer end %d %d\n", r, tr);
	tr = data.size();
	r = libusb_bulk_transfer(device->GetHandle(), in->GetAddress(), data.data(), data.size(), &tr, 3000);
	//r = libusb_interrupt_transfer(device->GetHandle(), interrupt->GetAddress(), data.data(), data.size(), &tr, 2000);
	printf("bulk transfer end %d %d\n", r, tr);
	for(size_t i = 0; i < data.size(); ++i)
	{
		u8 byte = data[i];
		printf("%02x ", byte);
	}
	printf("\n");
	Stream stream(data, 12);
	GetDeviceInfo gdi;
	gdi.Read(stream);
	printf("%s\n", gdi.VendorExtensionDesc.c_str());
	printf("%s\n", gdi.Manufactorer.c_str());
	printf("%s\n", gdi.Model.c_str());
	printf("%s\n", gdi.DeviceVersion.c_str());
	printf("%s\n", gdi.SerialNumber.c_str());
	for(u16 code : gdi.OperationsSupported)
	{
		printf("%04x\n", (unsigned)code);
	}
#endif
	libusb_release_interface(device->GetHandle(), interface->GetIndex());

	return 0;
}
