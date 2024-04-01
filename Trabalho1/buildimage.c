/* Author(s): <Your name(s) here>
 * Creates operating system image suitable for placement on a boot disk
 */
/* TODO: Comment on the status of your submission. Largely unimplemented */
#include <assert.h>
#include <elf.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IMAGE_FILE "./image"
#define ARGS "[--extended] <bootblock> <executable-file> ..."

#define SECTOR_SIZE 512				/* floppy sector size in bytes */
#define BOOTLOADER_SIG_OFFSET 0x1fe /* offset for boot loader signature */
// more defines...

/* Reads in an executable file in ELF format*/
Elf32_Phdr *read_exec_file(FILE **execfile, char *filename, Elf32_Ehdr **ehdr)
{
	Elf32_Phdr *phdr = malloc(sizeof(Elf32_Phdr));
	Elf32_Ehdr *header = malloc(sizeof(Elf32_Ehdr));

	/* open file */
	*execfile = fopen(filename, "r");
	if (*execfile == NULL)
	{
		fprintf(stderr, "Error: Unable to open file %s\n", filename);
		exit(1);
	}

	/* read ELF header */
	fread(header, sizeof(Elf32_Ehdr), 1, *execfile);

	/* check ELF header */
	if (header->e_ident[0] != 0x7f || header->e_ident[1] != 'E' || header->e_ident[2] != 'L' || header->e_ident[3] != 'F')
	{
		fprintf(stderr, "Error: Not an ELF file\n");
		exit(1);
	}

	/* read program header */
	fseek(*execfile, header->e_phoff, SEEK_SET);
	fread(phdr, sizeof(Elf32_Phdr), 1, *execfile);

	*ehdr = header;

	return phdr;
}

/* Writes the bootblock to the image file */
void write_bootblock(FILE **imagefile, FILE *bootfile, Elf32_Ehdr *boot_header, Elf32_Phdr *boot_phdr)
{
	// Calculate the number of sectors needed to store the bootblock
	int num_sec = (boot_phdr->p_filesz + SECTOR_SIZE - 1) / SECTOR_SIZE;

	// Allocate a buffer to hold the bootblock data
	char *buffer = malloc(num_sec * SECTOR_SIZE);
	if (buffer == NULL)
	{
		fprintf(stderr, "Error: Unable to allocate memory\n");
		exit(1);
	}

	// Read the bootblock data from the bootfile
	fseek(bootfile, boot_phdr->p_offset, SEEK_SET);
	fread(buffer, boot_phdr->p_filesz, 1, bootfile);

	// Write the bootblock data to the image file
	fwrite(buffer, num_sec * SECTOR_SIZE, 1, *imagefile);

	// Free the buffer
	free(buffer);
}

/* Writes the kernel to the image file */
void write_kernel(FILE **imagefile, FILE *kernelfile, Elf32_Ehdr *kernel_header, Elf32_Phdr *kernel_phdr)
{
	// Calculate the number of sectors needed to store the kernel
	int num_sec = (kernel_phdr->p_filesz + SECTOR_SIZE - 1) / SECTOR_SIZE;

	// Allocate a buffer to hold the kernel data
	char *buffer = malloc(num_sec * SECTOR_SIZE);
	if (buffer == NULL)
	{
		fprintf(stderr, "Error: Unable to allocate memory\n");
		exit(1);
	}

	// Read the kernel data from the kernelfile
	fseek(kernelfile, kernel_phdr->p_offset, SEEK_SET);
	fread(buffer, kernel_phdr->p_filesz, 1, kernelfile);

	// Write the kernel data to the image file
	fwrite(buffer, num_sec * SECTOR_SIZE, 1, *imagefile);

	// Free the buffer
	free(buffer);
}

/* Counts the number of sectors in the kernel */
int count_kernel_sectors(Elf32_Ehdr *kernel_header, Elf32_Phdr *kernel_phdr)
{
	// Calculate the number of sectors needed to store the kernel
	int num_sec = (kernel_phdr->p_filesz + SECTOR_SIZE - 1) / SECTOR_SIZE;

	return num_sec;
}

/* Records the number of sectors in the kernel */
void record_kernel_sectors(FILE **imagefile, Elf32_Ehdr *kernel_header, Elf32_Phdr *kernel_phdr, int num_sec)
{
	// Calculate the offset for recording the number of sectors
	int offset = BOOTLOADER_SIG_OFFSET - sizeof(int);

	// Seek to the offset in the image file
	fseek(*imagefile, offset, SEEK_SET);

	// Write the number of sectors to the image file
	fwrite(&num_sec, sizeof(int), 1, *imagefile);
}

/* Prints segment information for --extended option */
void extended_opt(Elf32_Phdr *bph, int k_phnum, Elf32_Phdr *kph, int num_sec)
{

	/* print number of disk sectors used by the image */
	printf("Number of disk sectors used by the image: %d\n", num_sec);

	/* bootblock segment info */
	printf("Bootblock segment info:\n");
	printf("Type: %d\n", bph->p_type);
	printf("Offset: %d\n", bph->p_offset);
	printf("Vaddr: %d\n", bph->p_vaddr);
	printf("Paddr: %d\n", bph->p_paddr);
	printf("Filesz: %d\n", bph->p_filesz);
	printf("Memsz: %d\n", bph->p_memsz);
	printf("Flags: %d\n", bph->p_flags);
	printf("Align: %d\n", bph->p_align);

	/* print kernel segment info */
	printf("Kernel segment info:\n");
	printf("Type: %d\n", kph->p_type);
	printf("Offset: %d\n", kph->p_offset);
	printf("Vaddr: %d\n", kph->p_vaddr);
	printf("Paddr: %d\n", kph->p_paddr);
	printf("Filesz: %d\n", kph->p_filesz);
	printf("Memsz: %d\n", kph->p_memsz);
	printf("Flags: %d\n", kph->p_flags);
	printf("Align: %d\n", kph->p_align);

	/* print kernel size in sectors */
	printf("Kernel size in sectors: %d\n", num_sec);
}
// more helper functions...

/* MAIN */
int main(int argc, char **argv)
{
	FILE *kernelfile, *bootfile, *imagefile;				// file pointers for bootblock,kernel and image
	Elf32_Ehdr *boot_header = malloc(sizeof(Elf32_Ehdr));	// bootblock ELF header
	Elf32_Ehdr *kernel_header = malloc(sizeof(Elf32_Ehdr)); // kernel ELF header

	Elf32_Phdr *boot_program_header;   // bootblock ELF program header
	Elf32_Phdr *kernel_program_header; // kernel ELF program header

	/* build image file */
	imagefile = fopen(IMAGE_FILE, "w");
	if (imagefile == NULL)
	{
		fprintf(stderr, "Error: Unable to open file %s\n", IMAGE_FILE);
		exit(1);
	}

	boot_program_header = read_exec_file(&bootfile, argv[1], &boot_header);

	// Print program header information

	printf("Bootblock program header information:\n");
	printf("Type: %d\n", boot_program_header->p_type);
	printf("Offset: %d\n", boot_program_header->p_offset);
	printf("Vaddr: %d\n", boot_program_header->p_vaddr);
	printf("Paddr: %d\n", boot_program_header->p_paddr);
	printf("Filesz: %d\n", boot_program_header->p_filesz);
	printf("Memsz: %d\n", boot_program_header->p_memsz);
	printf("Flags: %d\n", boot_program_header->p_flags);
	printf("Align: %d\n", boot_program_header->p_align);

	kernel_program_header = read_exec_file(&kernelfile, argv[2], &kernel_header);

	// Print program header information

	printf("Kernel program header information:\n");
	printf("Type: %d\n", kernel_program_header->p_type);
	printf("Offset: %d\n", kernel_program_header->p_offset);
	printf("Vaddr: %d\n", kernel_program_header->p_vaddr);
	printf("Paddr: %d\n", kernel_program_header->p_paddr);
	printf("Filesz: %d\n", kernel_program_header->p_filesz);
	printf("Memsz: %d\n", kernel_program_header->p_memsz);
	printf("Flags: %d\n", kernel_program_header->p_flags);
	printf("Align: %d\n", kernel_program_header->p_align);

	/* write bootblock */
	write_bootblock(&imagefile, bootfile, boot_header, boot_program_header);

	/* write kernel segments to image */
	write_kernel(&imagefile, kernelfile, kernel_header, kernel_program_header);

	/* tell the bootloader how many sectors to read to load the kernel */
	int num_sec = count_kernel_sectors(kernel_header, kernel_program_header);
	record_kernel_sectors(&imagefile, kernel_header, kernel_program_header, num_sec);

	/* check for --extended option */
	if (argc > 3 && !strncmp(argv[3], "--extended", 11))
	{
		/* print info */
		extended_opt(boot_program_header, argc - 3, kernel_program_header, num_sec);
	}

	return 0;
} // ends main()
