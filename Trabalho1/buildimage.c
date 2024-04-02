/* Author(s): Luiz Gustavo S. S. Junqueira, Camila Cardoso Cáceres
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

	*execfile = fopen(filename, "r");
	if (*execfile == NULL)
	{
		printf("Erro: Não foi possível abrir o arquivo %s\n", filename);
		return NULL;
	}

	// Lê o cabeçalho ELF do arquivo
	fread(*ehdr, sizeof(Elf32_Ehdr), 1, *execfile);

	// Verifica se o arquivo é um arquivo ELF
	if ((*ehdr)->e_ident[0] != 0x7f || (*ehdr)->e_ident[1] != 'E' || (*ehdr)->e_ident[2] != 'L' || (*ehdr)->e_ident[3] != 'F')
	{
		printf("Erro: Não é um arquivo ELF\n");
		return NULL;
	}

	// Move o ponteiro de escrita para o início do cabeçalho do programa
	fseek(*execfile, (*ehdr)->e_phoff, SEEK_SET);
	// Lê o cabeçalho do programa
	fread(phdr, sizeof(Elf32_Phdr), 1, *execfile);

	return phdr;
}

/* Writes the bootblock to the image file */
void write_bootblock(FILE **imagefile, FILE *bootfile, Elf32_Ehdr *boot_header, Elf32_Phdr *boot_phdr)
{

	int num_sec = (boot_phdr->p_filesz + SECTOR_SIZE - 1) / SECTOR_SIZE;

	/* write bootblock to image */

	char *buffer = malloc(num_sec * SECTOR_SIZE);
	fseek(bootfile, boot_phdr->p_offset, SEEK_SET);
	fread(buffer, boot_phdr->p_filesz, 1, bootfile);

	fwrite(buffer, num_sec * SECTOR_SIZE, 1, *imagefile);

	free(buffer);
}

/* Writes the kernel to the image file */
void write_kernel(FILE **imagefile, FILE *kernelfile, Elf32_Ehdr *kernel_header, Elf32_Phdr *kernel_phdr)
{
}

/* Counts the number of sectors in the kernel */
int count_kernel_sectors(Elf32_Ehdr *kernel_header, Elf32_Phdr *kernel_phdr)
{
	return 0;
}

/* Records the number of sectors in the kernel */
void record_kernel_sectors(FILE **imagefile, Elf32_Ehdr *kernel_header, Elf32_Phdr *kernel_phdr, int num_sec)
{
}

/* Prints segment information for --extended option */
void extended_opt(Elf32_Phdr *bph, int k_phnum, Elf32_Phdr *kph, int num_sec)
{

	/* print number of disk sectors used by the image */

	/* bootblock segment info */

	/* print kernel segment info */

	/* print kernel size in sectors */
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
		printf("Erro: Não foi possível criar o arquivo %s\n", IMAGE_FILE);
		return 1;
	}

	/* read executable bootblock file */

	boot_program_header = read_exec_file(&bootfile, argv[1], &boot_header);

	printf("Bootblock program header information:\n");
	printf("Type: %d\n", boot_program_header->p_type);
	printf("Offset: %d\n", boot_program_header->p_offset);
	printf("Vaddr: %d\n", boot_program_header->p_vaddr);
	printf("Paddr: %d\n", boot_program_header->p_paddr);
	printf("Filesz: %d\n", boot_program_header->p_filesz);
	printf("Memsz: %d\n", boot_program_header->p_memsz);
	printf("Flags: %d\n", boot_program_header->p_flags);
	printf("Align: %d\n", boot_program_header->p_align);

	/* write bootblock */

	write_bootblock(&imagefile, bootfile, boot_header, boot_program_header);

	/*Write image file signature*/
	fseek(imagefile, BOOTLOADER_SIG_OFFSET, SEEK_SET);
	fwrite("\x55\xAA", 2, 1, imagefile);
	

	/* read executable kernel file */

	kernel_program_header = read_exec_file(&kernelfile, argv[2], &kernel_header);

	printf("Kernel program header information:\n");
	printf("Type: %d\n", kernel_program_header->p_type);
	printf("Offset: %d\n", kernel_program_header->p_offset);
	printf("Vaddr: %d\n", kernel_program_header->p_vaddr);
	printf("Paddr: %d\n", kernel_program_header->p_paddr);
	printf("Filesz: %d\n", kernel_program_header->p_filesz);
	printf("Memsz: %d\n", kernel_program_header->p_memsz);
	printf("Flags: %d\n", kernel_program_header->p_flags);
	printf("Align: %d\n", kernel_program_header->p_align);

	/* write kernel segments to image */

	/* tell the bootloader how many sectors to read to load the kernel */

	/* check for  --extended option */
	if (!strncmp(argv[1], "--extended", 11))
	{
		/* print info */
		extended_opt(boot_program_header, kernel_header->e_phnum, kernel_program_header, count_kernel_sectors(kernel_header, kernel_program_header));
	}

	return 0;
} // ends main()