/* SPDX-License-Identifier: GPL-2.0 */
/**
 * drivers/block/zram/internal.h - ZRAM Depend Func Wrapper
 * @Desc: Some of the functions we depend on cannot be directly
 *        applied to the kernel. which would break the KABI of
 *        the Android Common Kernel Tree. Therefore, we need to
 *        implement the functions that ZRAM depends internal.h.
 * @Author: zhaoyuenan <amktiao030215@gmail.com>
 */

#ifdef MODULE
static void zram_bio_add_page(struct bio *bio, struct page *page,
		unsigned int len, unsigned int off)
{
	struct bio_vec *bv = &bio->bi_io_vec[bio->bi_vcnt];

	WARN_ON_ONCE(bio_flagged(bio, BIO_CLONED));
	WARN_ON_ONCE(bio_full(bio, len));

	bv->bv_page = page;
	bv->bv_offset = off;
	bv->bv_len = len;

	bio->bi_iter.bi_size += len;
	bio->bi_vcnt++;

	if (!bio_flagged(bio, BIO_WORKINGSET) && unlikely(PageWorkingset(page)))
		bio_set_flag(bio, BIO_WORKINGSET);
}
#else
#define zram_bio_add_page __bio_add_page
#endif

/*
 * NOTE (6.6 port): bvec_iter_advance_single(), bio_advance_iter_single(),
 * memcpy_from_bvec() and memcpy_to_bvec() are already provided by
 * include/linux/bvec.h and include/linux/bio.h on 6.6, so keeping local
 * copies here triggers "redefinition" errors.  Only the bio_add_page
 * wrapper above is still needed for the module build.
 */
