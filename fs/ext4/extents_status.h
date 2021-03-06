/*
 *  fs/ext4/extents_status.h
 *
 * Written by Yongqiang Yang <xiaoqiangnk@gmail.com>
 * Modified by
 *	Allison Henderson <achender@linux.vnet.ibm.com>
 *	Zheng Liu <wenqing.lz@taobao.com>
 *
 */

#ifndef _EXT4_EXTENTS_STATUS_H
#define _EXT4_EXTENTS_STATUS_H

/*
 * Turn on ES_DEBUG__ to get lots of info about extent status operations.
 */
#ifdef ES_DEBUG__
#define es_debug(fmt, ...)	printk(fmt, ##__VA_ARGS__)
#else
#define es_debug(fmt, ...)	no_printk(fmt, ##__VA_ARGS__)
#endif

#define EXTENT_STATUS_WRITTEN	0x80000000	/* written extent */
#define EXTENT_STATUS_UNWRITTEN	0x40000000	/* unwritten extent */
#define EXTENT_STATUS_DELAYED	0x20000000	/* delayed extent */
#define EXTENT_STATUS_HOLE	0x10000000	/* hole */

#define EXTENT_STATUS_FLAGS	(EXTENT_STATUS_WRITTEN | \
				 EXTENT_STATUS_UNWRITTEN | \
				 EXTENT_STATUS_DELAYED | \
				 EXTENT_STATUS_HOLE)

struct extent_status {
	struct rb_node rb_node;
	ext4_lblk_t es_lblk;	/* first logical block extent covers */
	ext4_lblk_t es_len;	/* length of extent in block */
	ext4_fsblk_t es_pblk;	/* first physical block */
};

struct ext4_es_tree {
	struct rb_root root;
	struct extent_status *cache_es;	/* recently accessed extent */
};

extern int __init ext4_init_es(void);
extern void ext4_exit_es(void);
extern void ext4_es_init_tree(struct ext4_es_tree *tree);

extern int ext4_es_insert_extent(struct inode *inode, ext4_lblk_t lblk,
				 ext4_lblk_t len, ext4_fsblk_t pblk,
				 unsigned long long status);
extern int ext4_es_remove_extent(struct inode *inode, ext4_lblk_t lblk,
				 ext4_lblk_t len);
extern void ext4_es_find_delayed_extent(struct inode *inode, ext4_lblk_t lblk,
					struct extent_status *es);
extern int ext4_es_lookup_extent(struct inode *inode, ext4_lblk_t lblk,
				 struct extent_status *es);

static inline int ext4_es_is_written(struct extent_status *es)
{
	return (es->es_pblk & EXTENT_STATUS_WRITTEN);
}

static inline int ext4_es_is_unwritten(struct extent_status *es)
{
	return (es->es_pblk & EXTENT_STATUS_UNWRITTEN);
}

static inline int ext4_es_is_delayed(struct extent_status *es)
{
	return (es->es_pblk & EXTENT_STATUS_DELAYED);
}

static inline int ext4_es_is_hole(struct extent_status *es)
{
	return (es->es_pblk & EXTENT_STATUS_HOLE);
}

static inline ext4_fsblk_t ext4_es_status(struct extent_status *es)
{
	return (es->es_pblk & EXTENT_STATUS_FLAGS);
}

static inline ext4_fsblk_t ext4_es_pblock(struct extent_status *es)
{
	return (es->es_pblk & ~EXTENT_STATUS_FLAGS);
}

static inline void ext4_es_store_pblock(struct extent_status *es,
					ext4_fsblk_t pb)
{
	ext4_fsblk_t block;

	block = (pb & ~EXTENT_STATUS_FLAGS) |
		(es->es_pblk & EXTENT_STATUS_FLAGS);
	es->es_pblk = block;
}

static inline void ext4_es_store_status(struct extent_status *es,
					unsigned long long status)
{
	ext4_fsblk_t block;

	block = (status & EXTENT_STATUS_FLAGS) |
		(es->es_pblk & ~EXTENT_STATUS_FLAGS);
	es->es_pblk = block;
}

extern void ext4_es_register_shrinker(struct super_block *sb);
extern void ext4_es_unregister_shrinker(struct super_block *sb);
extern void ext4_es_lru_add(struct inode *inode);
extern void ext4_es_lru_del(struct inode *inode);

#endif /* _EXT4_EXTENTS_STATUS_H */
