#include "DbfsManager.hpp"
// Definitions are in `DbfsManager.hpp`
namespace shiku
{
	size_t SizeOfDatafile(int index)
	{
		// .0 => 32MiB, .1 => 64MiB, ...	
		// (0x1 << 5) == 32, 32MiB == 33554432 Bytes (33554432 = 32 * 1024 * 1024)
		// (0x1 << i + 5) << 10 << 10 = 0x1 << i + 25
		// The 7th file reaches 2GiB and stops growing
		return index >= 7 ? 0x1 << 31 /*2GiB*/ : 0x1 << (index + 25);
	}
	DiskLoc Allocator::MALLOC(DbfsManager &mgr, size_t size)
	{
		using byte = char;
		DiskLoc ret;
		// Linear traverse the freelist
		Record *p = (Record*)mgr.GetAddrFromDl(*mgr.freelist);
		while(!p->self.IsNullLoc())
		{
			// There're `Record` properties (32 Bytes)
			if(p->length + sizeof(Record) >= size)
			{
				// Delete itself from freelist
				if(!p->prev.IsNullLoc())
					((Record*)mgr.GetAddrFromDl(p->prev))->next = p->next;
				if(!p->next.IsNullLoc())
					((Record*)mgr.GetAddrFromDl(p->next))->prev = p->prev;
				// Update `mgr.freelist`
				if(p->self == *mgr.freelist)
					*mgr.freelist = p->next;
				if(p->length > size + 128) // Assume that 128 Bytes is the minimum size to realloc
				{
					Record *newrec = (Record*)((byte*)p + sizeof(Record) + size); // 32 == sizeof(Record)
					newrec->self.Assign(p->self.file, p->self.offset + sizeof(Record) + size);
					newrec->length = p->length - sizeof(Record) - size;
					FREE(mgr, *newrec);
				}
				return p->self;
			}
			p = (Record*)mgr.GetAddrFromDl(p->next);
		}
		// Create a new data file or not?
		if(mgr.lastAvail->offset + size > SizeOfDatafile(mgr.lastAvail->file))
		{
			// Check if the unused part is long enough
			if(SizeOfDatafile(mgr.lastAvail->file) - mgr.lastAvail->offset >= 48) // A `Record` uses 32 Bytes
			{
				// Construct a `Record` (just `Record.self` and `Record.length`)
				Record *rec = (Record*)mgr.lastAvail;
				rec->length = SizeOfDatafile(mgr.lastAvail->file) - mgr.lastAvail->offset - sizeof(Record);
				// Free the unused part
				Allocator::FREE(mgr, *rec);
			}
			// Create a new file
			mgr.CreateNewDatafile();
			// Set `ret`'s property
			ret.file = *mgr.DataFileCount - 1;
			ret.offset = 0;
		}
		else
			ret = *mgr.lastAvail;
		// Set mgr.lastAvail
		mgr.lastAvail->file = ret.file;
		mgr.lastAvail->offset = ret.offset + size;
		return ret;
	}
	void Allocator::FREE(const DbfsManager &mgr, Record &rec)
	{
		// Freelist index is not supported now
		// Simply add `rec` to freelist's head
		rec.next = *mgr.freelist;
		rec.prev = DiskLoc(); // Default constructor makes a NullLoc DL
		*mgr.freelist = rec.self;
	}
}