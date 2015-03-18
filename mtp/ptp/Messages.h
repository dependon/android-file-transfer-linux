#ifndef MESSAGES_H
#define	MESSAGES_H

#include <mtp/ptp/InputStream.h>
#include <mtp/ptp/OutputStream.h>
#include <mtp/ptp/ObjectFormat.h>

namespace mtp { namespace msg
{
	struct DeviceInfo
	{
		u16					StandardVersion;
		u32					VendorExtensionId;
		u16					VendorExtensionVersion;
		std::string			VendorExtensionDesc;
		u16					FunctionalMode;
		std::vector<u16>	OperationsSupported;
		std::vector<u16>	EventsSupported;
		std::vector<u16>	DevicePropertiesSupported;
		std::vector<u16>	CaptureFormats;
		std::vector<u16>	ImageFormats;
		std::string			Manufactorer;
		std::string			Model;
		std::string			DeviceVersion;
		std::string			SerialNumber;

		void Read(InputStream &stream)
		{
			stream >> StandardVersion;
			stream >> VendorExtensionId;
			stream >> VendorExtensionVersion;
			stream >> VendorExtensionDesc;
			stream >> FunctionalMode;
			stream >> OperationsSupported;
			stream >> EventsSupported;
			stream >> DevicePropertiesSupported;
			stream >> CaptureFormats;
			stream >> ImageFormats;
			stream >> Manufactorer;
			stream >> Model;
			stream >> DeviceVersion;
			stream >> SerialNumber;
		}
	};

	struct ObjectHandles
	{
		std::vector<u32> ObjectHandles;

		void Read(InputStream &stream)
		{
			stream >> ObjectHandles;
		}
	};

	struct StorageIDs
	{
		std::vector<u32> StorageIDs;

		void Read(InputStream &stream)
		{
			stream >> StorageIDs;
		}
	};

	struct StorageInfo
	{
		u16			StorageType;
		u16			FilesystemType;
		u16			AccessCapability;
		u64			MaxCapacity;
		u64			FreeSpaceInBytes;
		u32			FreeSpaceInImages;
		std::string	StorageDescription;
		std::string	VolumeLabel;


		void Read(InputStream &stream)
		{
			stream >> StorageType;
			stream >> FilesystemType;
			stream >> AccessCapability;
			stream >> MaxCapacity;
			stream >> FreeSpaceInBytes;
			stream >> FreeSpaceInImages;
			stream >> StorageDescription;
			stream >> VolumeLabel;
		}
	};

	struct ObjectInfo
	{
		u32					StorageId;
		mtp::ObjectFormat	ObjectFormat;
		u16					ProtectionStatus;
		u32					ObjectCompressedSize;
		u16					ThumbFormat;
		u32					ThumbCompressedSize;
		u32					ThumbPixWidth;
		u32					ThumbPixHeight;
		u32					ImagePixWidth;
		u32					ImagePixHeight;
		u32					ImageBitDepth;
		u32					ParentObject;
		u16					AssociationType;
		u32					AssociationDesc;
		u32					SequenceNumber;
		std::string			Filename;
		std::string			CaptureDate;
		std::string			ModificationDate;
		std::string			Keywords;

		ObjectInfo(): StorageId(), ObjectFormat(), ProtectionStatus(), ObjectCompressedSize(),
			ThumbFormat(), ThumbCompressedSize(), ThumbPixWidth(), ThumbPixHeight(),
			ImagePixWidth(), ImagePixHeight(), ImageBitDepth(),
			ParentObject(), AssociationType(), AssociationDesc(),
			SequenceNumber()
		{ }

		void Read(InputStream &stream)
		{
			stream >> StorageId;
			stream >> ObjectFormat;
			stream >> ProtectionStatus;
			stream >> ObjectCompressedSize;
			stream >> ThumbFormat;
			stream >> ThumbCompressedSize;
			stream >> ThumbPixWidth;
			stream >> ThumbPixHeight;
			stream >> ImagePixWidth;
			stream >> ImagePixHeight;
			stream >> ImageBitDepth;
			stream >> ParentObject;
			stream >> AssociationType;
			stream >> AssociationDesc;
			stream >> SequenceNumber;
			stream >> Filename;
			stream >> CaptureDate;
			stream >> ModificationDate;
			stream >> Keywords;
		}
//fixme: make me template
		void Write(OutputStream &stream) const
		{
			stream << StorageId;
			stream << ObjectFormat;
			stream << ProtectionStatus;
			stream << ObjectCompressedSize;
			stream << ThumbFormat;
			stream << ThumbCompressedSize;
			stream << ThumbPixWidth;
			stream << ThumbPixHeight;
			stream << ImagePixWidth;
			stream << ImagePixHeight;
			stream << ImageBitDepth;
			stream << ParentObject;
			stream << AssociationType;
			stream << AssociationDesc;
			stream << SequenceNumber;
			stream << Filename;
			stream << CaptureDate;
			stream << ModificationDate;
			stream << Keywords;
		}
	};

	struct ObjectPropsSupported
	{
		std::vector<u16>	ObjectPropCodes;

		void Read(InputStream &stream)
		{
			stream >> ObjectPropCodes;
		}
	};

}}


#endif	/* MESSAGES_H */
