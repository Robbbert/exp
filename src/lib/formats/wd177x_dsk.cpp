// license:BSD-3-Clause
// copyright-holders:Olivier Galibert, 68bit
/*********************************************************************

    formats/wd177x_dsk.h

    helper for simple wd177x-formatted disk images

*********************************************************************/

#include "emu.h" // emu_fatalerror
#include "formats/wd177x_dsk.h"

wd177x_format::wd177x_format(const format *_formats)
{
	formats = _formats;
}

// Default implementation. May be overwritten by subclasses to handle tracks
// that vary from the default, such as a FM encoded track on a largely MFM
// encoded disk, or a track with an different sector IDs etc. Only the track
// encoding is used from the returned format, the number of track_count is not
// accessed.
const wd177x_format::format &wd177x_format::get_track_format(const format &f, int head, int track)
{
	return f;
}

/*
    Default implementation for find_size. May be overwritten by subclasses.
*/
int wd177x_format::find_size(io_generic *io, uint32_t form_factor)
{
	uint64_t size = io_generic_size(io);
	for(int i=0; formats[i].form_factor; i++) {
		const format &f = formats[i];
		if(form_factor != floppy_image::FF_UNKNOWN && form_factor != f.form_factor)
			continue;

		uint64_t format_size = 0;
		for(int track=0; track < f.track_count; track++) {
			for(int head=0; head < f.head_count; head++) {
				const format &tf = get_track_format(f, head, track);
				format_size += compute_track_size(tf);
			}
		}

		if(size == format_size)
			return i;
	}
	return -1;
}

int wd177x_format::identify(io_generic *io, uint32_t form_factor)
{
	int type = find_size(io, form_factor);

	if(type != -1)
		return 50;
	return 0;
}

// A track specific format is to be supplied.
int wd177x_format::compute_track_size(const format &f) const
{
	int track_size;
	if(f.sector_base_size)
		track_size = f.sector_base_size * f.sector_count;
	else {
		track_size = 0;
		for(int i=0; i != f.sector_count; i++)
			track_size += f.per_sector_size[i];
	}
	return track_size;
}

// A track specific format is to be supplied.
void wd177x_format::build_sector_description(const format &f, uint8_t *sectdata, desc_s *sectors, int track, int head) const
{
	if(f.sector_base_id == -1) {
		for(int i=0; i<f.sector_count; i++) {
			int cur_offset = 0;
			for(int j=0; j<f.sector_count; j++)
				if(f.per_sector_id[j] < f.per_sector_id[i])
					cur_offset += f.sector_base_size ? f.sector_base_size : f.per_sector_size[j];
			sectors[i].data = sectdata + cur_offset;
			sectors[i].size = f.sector_base_size ? f.sector_base_size : f.per_sector_size[i];
			sectors[i].sector_id = f.per_sector_id[i];
		}
	} else {
		int cur_offset = 0;
		for(int i=0; i<f.sector_count; i++) {
			sectors[i].data = sectdata + cur_offset;
			sectors[i].size = f.sector_base_size ? f.sector_base_size : f.per_sector_size[i];
			cur_offset += sectors[i].size;
			sectors[i].sector_id = i + f.sector_base_id;
		}
	}
}

// A track specific format is to be supplied.
floppy_image_format_t::desc_e* wd177x_format::get_desc_fm(const format &f, int &current_size, int &end_gap_index)
{
	static floppy_image_format_t::desc_e desc[23] = {
		/* 00 */ { FM, 0xff, 0 },
		/* 01 */ { SECTOR_LOOP_START, 0, 0 },
		/* 02 */ {   FM, 0x00, 6 },
		/* 03 */ {   CRC_CCITT_FM_START, 1 },
		/* 04 */ {     RAW, 0xf57e, 1 },
		/* 05 */ {     TRACK_ID_FM },
		/* 06 */ {     HEAD_ID_FM },
		/* 07 */ {     SECTOR_ID_FM },
		/* 08 */ {     SIZE_ID_FM },
		/* 09 */ {   CRC_END, 1 },
		/* 10 */ {   CRC, 1 },
		/* 11 */ {   FM, 0xff, 0 },
		/* 12 */ {   FM, 0x00, 6 },
		/* 13 */ {   CRC_CCITT_FM_START, 2 },
		/* 14 */ {     RAW, 0xf56f, 1 },
		/* 15 */ {     SECTOR_DATA_FM, -1 },
		/* 16 */ {   CRC_END, 2 },
		/* 17 */ {   CRC, 2 },
		/* 18 */ {   FM, 0xff, 0 },
		/* 19 */ { SECTOR_LOOP_END },
		/* 20 */ { FM, 0xff, 0 },
		/* 21 */ { RAWBITS, 0xffff, 0 },
		/* 22 */ { END }
	};

	desc[0].p2 = f.gap_1;
	desc[1].p2 = f.sector_count - 1;
	desc[11].p2 = f.gap_2;
	desc[18].p2 = f.gap_3;

	current_size = f.gap_1*16;
	if(f.sector_base_size)
		current_size += f.sector_base_size * f.sector_count * 16;
	else {
		for(int j=0; j != f.sector_count; j++)
			current_size += f.per_sector_size[j] * 16;
	}
	current_size += (6+1+4+2+f.gap_2+6+1+2+f.gap_3) * f.sector_count * 16;

	end_gap_index = 20;

	return desc;
}

// A track specific format is to be supplied.
floppy_image_format_t::desc_e* wd177x_format::get_desc_mfm(const format &f, int &current_size, int &end_gap_index)
{
	static floppy_image_format_t::desc_e desc[25] = {
		/* 00 */ { MFM, 0x4e, 0 },
		/* 01 */ { SECTOR_LOOP_START, 0, 0 },
		/* 02 */ {   MFM, 0x00, 12 },
		/* 03 */ {   CRC_CCITT_START, 1 },
		/* 04 */ {     RAW, 0x4489, 3 },
		/* 05 */ {     MFM, 0xfe, 1 },
		/* 06 */ {     TRACK_ID },
		/* 07 */ {     HEAD_ID },
		/* 08 */ {     SECTOR_ID },
		/* 09 */ {     SIZE_ID },
		/* 10 */ {   CRC_END, 1 },
		/* 11 */ {   CRC, 1 },
		/* 12 */ {   MFM, 0x4e, 0 },
		/* 13 */ {   MFM, 0x00, 12 },
		/* 14 */ {   CRC_CCITT_START, 2 },
		/* 15 */ {     RAW, 0x4489, 3 },
		/* 16 */ {     MFM, 0xfb, 1 },
		/* 17 */ {     SECTOR_DATA, -1 },
		/* 18 */ {   CRC_END, 2 },
		/* 19 */ {   CRC, 2 },
		/* 20 */ {   MFM, 0x4e, 0 },
		/* 21 */ { SECTOR_LOOP_END },
		/* 22 */ { MFM, 0x4e, 0 },
		/* 23 */ { RAWBITS, 0x9254, 0 },
		/* 24 */ { END }
	};

	desc[0].p2 = f.gap_1;
	desc[1].p2 = f.sector_count - 1;
	desc[12].p2 = f.gap_2;
	desc[20].p2 = f.gap_3;

	current_size = f.gap_1*16;
	if(f.sector_base_size)
		current_size += f.sector_base_size * f.sector_count * 16;
	else {
		for(int j=0; j != f.sector_count; j++)
			current_size += f.per_sector_size[j] * 16;
	}
	current_size += (12+3+1+4+2+f.gap_2+12+3+1+2+f.gap_3) * f.sector_count * 16;

	end_gap_index = 22;

	return desc;
}

bool wd177x_format::load(io_generic *io, uint32_t form_factor, floppy_image *image)
{
	int type = find_size(io, form_factor);
	if(type == -1)
		return false;

	const format &f = formats[type];

	for(int track=0; track < f.track_count; track++)
		for(int head=0; head < f.head_count; head++) {
			uint8_t sectdata[40*512];
			desc_s sectors[40];
			floppy_image_format_t::desc_e *desc;
			int current_size;
			int end_gap_index;
			const format &tf = get_track_format(f, head, track);

			switch (tf.encoding)
			{
			case floppy_image::FM:
				desc = get_desc_fm(tf, current_size, end_gap_index);
				break;
			case floppy_image::MFM:
			default:
				desc = get_desc_mfm(tf, current_size, end_gap_index);
				break;
			}

			int total_size = 200000000/tf.cell_size;
			int remaining_size = total_size - current_size;
			if(remaining_size < 0)
				throw emu_fatalerror("wd177x_format: Incorrect track layout, max_size=%d, current_size=%d", total_size, current_size);

			// Fixup the end gap
			desc[end_gap_index].p2 = remaining_size / 16;
			desc[end_gap_index + 1].p2 = remaining_size & 15;
			desc[end_gap_index + 1].p1 >>= 16-(remaining_size & 15);

			if (tf.encoding == floppy_image::FM)
				desc[14].p1 = get_track_dam_fm(tf, head, track);
			else
				desc[16].p1 = get_track_dam_mfm(tf, head, track);

			build_sector_description(tf, sectdata, sectors, track, head);
			int track_size = compute_track_size(tf);
			io_generic_read(io, sectdata, get_image_offset(f, head, track), track_size);
			generate_track(desc, track, head, sectors, tf.sector_count, total_size, image);
		}

	image->set_variant(f.variant);

	return true;
}

bool wd177x_format::supports_save() const
{
	return true;
}

bool wd177x_format::save(io_generic *io, floppy_image *image)
{
	// Count the number of formats
	int formats_count;
	for(formats_count=0; formats[formats_count].form_factor; formats_count++) {};

	// Allocate the storage for the list of testable formats for a
	// given cell size
	std::vector<int> candidates;

	// Format we're finally choosing
	int chosen_candidate = -1;

	// Previously tested cell size
	int min_cell_size = 0;
	for(;;) {
		// Build the list of all formats for the immediately superior cell size
		int cur_cell_size = 0;
		candidates.clear();
		for(int i=0; i < formats_count; i++) {
			if(image->get_form_factor() == floppy_image::FF_UNKNOWN ||
				image->get_form_factor() == formats[i].form_factor) {
				if(formats[i].cell_size == cur_cell_size)
					candidates.push_back(i);
				else if((!cur_cell_size || formats[i].cell_size < cur_cell_size) &&
						formats[i].cell_size > min_cell_size) {
					candidates.clear();
					candidates.push_back(i);
					cur_cell_size = formats[i].cell_size;
				}
			}
		}

		min_cell_size = cur_cell_size;

		// No candidates with a cell size bigger than the previously
		// tested one, we're done
		if(candidates.empty())
			break;

		// Filter
		check_compatibility(image, candidates);

		// Nobody matches, try with the next cell size
		if(candidates.empty())
			continue;

		// We have a match at that cell size, we just need to find the
		// best one given the geometry

		// If there's only one, we're done
		if(candidates.size() == 1) {
			chosen_candidate = candidates[0];
			break;
		}

		// Otherwise, find the best
		int tracks, heads;
		image->get_actual_geometry(tracks, heads);
		chosen_candidate = candidates[0];
		for(unsigned int i=1; i < candidates.size(); i++) {
			const format &cc = formats[chosen_candidate];
			const format &cn = formats[candidates[i]];

			// Handling enough sides is better than not
			if(cn.head_count >= heads && cc.head_count < heads)
				goto change;
			else if(cc.head_count >= heads && cn.head_count < heads)
				goto dont_change;

			// Handling enough tracks is better than not
			if(cn.track_count >= tracks && cc.track_count < tracks)
				goto change;
			else if(cc.track_count >= tracks && cn.track_count < tracks)
				goto dont_change;

			// Both are on the same side of the track count, so closest is best
			if(cc.track_count < tracks && cn.track_count > cc.track_count)
				goto change;
			if(cc.track_count >= tracks && cn.track_count < cc.track_count)
				goto change;

			// Lower number of heads is better
			if (cn.head_count < cc.head_count && cn.head_count <= heads)
				goto change;

			goto dont_change;

		change:
			chosen_candidate = candidates[i];
		dont_change:
			;
		}
		// We have a winner, bail out
		break;
	}

	// No match, pick the first one and be done with it
	if(chosen_candidate == -1)
		chosen_candidate = 0;

	const format &f = formats[chosen_candidate];

	uint8_t sectdata[40*512];
	desc_s sectors[40];

	for(int track=0; track < f.track_count; track++) {
		for(int head=0; head < f.head_count; head++) {
			const format &tf = get_track_format(f, head, track);
			build_sector_description(tf, sectdata, sectors, track, head);
			extract_sectors(image, tf, sectors, track, head);
			int track_size = compute_track_size(tf);
			io_generic_write(io, sectdata, get_image_offset(f, head, track), track_size);
		}
	}

	return true;
}

/*
    Default implementation of the image offset computation. May be overwritten
    by subclasses.
*/
int wd177x_format::get_image_offset(const format &f, int head, int track)
{
	int offset = 0;

	for(int trk=0; trk < track; trk++) {
		for(int hd=0; hd < f.head_count; hd++) {
			const format &tf = get_track_format(f, hd, trk);
			offset += compute_track_size(tf);
		}
	}

	for(int hd=0; hd < head; hd++) {
		const format &tf = get_track_format(f, hd, track);
		offset += compute_track_size(tf);
	}


	return offset;
}

// A track specific format is to be supplied.
int wd177x_format::get_track_dam_fm(const format &f, int head, int track)
{
	// everything marked as data by default
	return FM_DAM;
}

// A track specific format is to be supplied.
int wd177x_format::get_track_dam_mfm(const format &f, int head, int track)
{
	// everything marked as data by default
	return MFM_DAM;
}

void wd177x_format::check_compatibility(floppy_image *image, std::vector<int> &candidates)
{
	// Check compatibility with every candidate, copy in-place
	int *ok_cands = &candidates[0];
	for(unsigned int i=0; i < candidates.size(); i++) {
		const format &f = formats[candidates[i]];
		for(int track=0; track < f.track_count; track++) {
			for(int head=0; head < f.head_count; head++) {
				uint8_t bitstream[500000/8];
				uint8_t sectdata[50000];
				desc_xs sectors[256];
				int track_size;
				const format &tf = get_track_format(f, head, track);

				generate_bitstream_from_track(track, head, tf.cell_size, bitstream, track_size, image);

				switch (tf.encoding)
				{
				case floppy_image::FM:
					extract_sectors_from_bitstream_fm_pc(bitstream, track_size, sectors, sectdata, sizeof(sectdata));
					break;
				case floppy_image::MFM:
					extract_sectors_from_bitstream_mfm_pc(bitstream, track_size, sectors, sectdata, sizeof(sectdata));
					break;
				}
				int ns = 0;
				for(int j=0; j<256; j++)
					if(sectors[j].data) {
						int sid;
						if(tf.sector_base_id == -1) {
							for(sid=0; sid < tf.sector_count; sid++)
								if(tf.per_sector_id[sid] == j)
									break;
						} else
							sid = j - tf.sector_base_id;
						if(sid < 0 || sid > tf.sector_count)
							goto fail;
						if(tf.sector_base_size) {
							if(sectors[j].size != tf.sector_base_size)
								goto fail;
						} else {
							if(sectors[j].size != tf.per_sector_size[sid])
								goto fail;
						}
						ns++;
					}
				if(ns != tf.sector_count)
					goto fail;
			}
		}
		*ok_cands++ = candidates[i];
	fail:
		;
	}
	candidates.resize(ok_cands - &candidates[0]);
}

// A track specific format is to be supplied.
void wd177x_format::extract_sectors(floppy_image *image, const format &f, desc_s *sdesc, int track, int head)
{
	uint8_t bitstream[500000/8];
	uint8_t sectdata[50000];
	desc_xs sectors[256];
	int track_size;

	// Extract the sectors
	generate_bitstream_from_track(track, head, f.cell_size, bitstream, track_size, image);

	switch (f.encoding)
	{
	case floppy_image::FM:
		extract_sectors_from_bitstream_fm_pc(bitstream, track_size, sectors, sectdata, sizeof(sectdata));
		break;
	case floppy_image::MFM:
		extract_sectors_from_bitstream_mfm_pc(bitstream, track_size, sectors, sectdata, sizeof(sectdata));
		break;
	}

	for(int i=0; i<f.sector_count; i++) {
		desc_s &ds = sdesc[i];
		desc_xs &xs = sectors[ds.sector_id];
		if(!xs.data)
			memset((void *)ds.data, 0, ds.size);
		else if(xs.size < ds.size) {
			memcpy((void *)ds.data, xs.data, xs.size);
			memset((uint8_t *)ds.data + xs.size, 0, xs.size - ds.size);
		} else
			memcpy((void *)ds.data, xs.data, ds.size);
	}
}
