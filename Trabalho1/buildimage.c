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

	// Cria buffer para armazenar setor
	char *buffer = malloc(num_sec * SECTOR_SIZE);

	// Move o ponteiro de leitura para o início do programa do bootblock
	fseek(bootfile, boot_phdr->p_offset, SEEK_SET);

	// Le 1 setor do bootblock
	fread(buffer, boot_phdr->p_filesz, 1, bootfile);

	// Escreve 1 setor do bootblock no arquivo de imagem
	fwrite(buffer, num_sec * SECTOR_SIZE, 1, *imagefile);

	free(buffer);
}

/* Writes the kernel to the image file */
void write_kernel(FILE **imagefile, FILE *kernelfile, Elf32_Ehdr *kernel_header, Elf32_Phdr *kernel_phdr)
{
	int num_sec = (kernel_phdr->p_filesz + SECTOR_SIZE - 1) / SECTOR_SIZE;

	/* write kernel to image */

	// Cria buffer para armazenar setor
	char *buffer = malloc(num_sec * SECTOR_SIZE);

	// Move o ponteiro de leitura para o início do programa do kernel
	fseek(kernelfile, kernel_phdr->p_offset, SEEK_SET);

	// Le 1 setor do kernel
	fread(buffer, kernel_phdr->p_filesz, 1, kernelfile);

	// Escreve 1 setor do kernel no arquivo de imagem
	fwrite(buffer, num_sec * SECTOR_SIZE, 1, *imagefile);

	free(buffer);
}

/* Counts the number of sectors in the kernel */
int count_kernel_sectors(Elf32_Ehdr *kernel_header, Elf32_Phdr *kernel_phdr)
{
	// Calcula o número de setores necessários para armazenar o kernel, garantindo que o kernel ocupe um número inteiro de setores
	int num_sec = (kernel_phdr->p_filesz + SECTOR_SIZE - 1) / SECTOR_SIZE;
	return num_sec;
}

/* Records the number of sectors in the kernel */
void record_kernel_sectors(FILE **imagefile, Elf32_Ehdr *kernel_header, Elf32_Phdr *kernel_phdr, int num_sec)
{
	/* write kernel size in sectors */

	// Muda o ponteiro de escrita para o byte 2 do arquivo de imagem
	fseek(*imagefile, 2, SEEK_SET);

	// Escreve o número de setores do kernel no arquivo de imagem
	fwrite(&num_sec, sizeof(int), 1, *imagefile);
}

/* Prints segment information for --extended option */
void extended_opt(Elf32_Phdr *bph, int k_phnum, Elf32_Phdr *kph, int num_sec)
{

	printf("AA");
	printf("0x0000: ./bootblock\n");
	printf("\tsegment 0\n");
	printf("\t\toffset 0x%04x\t\tvaddr 0x%04x\n", bph->p_offset, bph->p_vaddr);
	printf("\t\tfilesz 0x%04x\t\tmemsz 0x%04x\n", bph->p_filesz, bph->p_memsz);
	printf("\t\twriting 0x%04x bytes\n", bph->p_filesz);

	int bootblockPadding = ((bph->p_offset + bph->p_filesz + SECTOR_SIZE - 1) / SECTOR_SIZE) * SECTOR_SIZE;
	printf("\t\tpadding up to 0x%04x\n", bootblockPadding);

	printf("0x1000: ./kernel\n");
	printf("\tsegment 0\n");
	printf("\t\toffset 0x%04x\t\tvaddr 0x%04x\n", kph->p_offset, kph->p_vaddr);
	printf("\t\tfilesz 0x%04x\t\tmemsz 0x%04x\n", kph->p_filesz, kph->p_memsz);
	printf("\t\twriting 0x%04x bytes\n", kph->p_filesz);

	int kernelPadding = ((kph->p_offset + kph->p_filesz + SECTOR_SIZE - 1) / SECTOR_SIZE) * SECTOR_SIZE + bootblockPadding;
	printf("\t\tpadding up to 0x%04x\n", kernelPadding);

	printf("os_size: %d sectors\n", num_sec);
}

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

	/* read executable bootblock and kernel file */

	// Get correct file name from arguments
	if (argc == 3)
	{
		boot_program_header = read_exec_file(&bootfile, argv[1], &boot_header);
		kernel_program_header = read_exec_file(&kernelfile, argv[2], &kernel_header);
	}
	else if (argc == 4)
	{
		boot_program_header = read_exec_file(&bootfile, argv[2], &boot_header);
		kernel_program_header = read_exec_file(&kernelfile, argv[3], &kernel_header);
	}

	/* write bootblock */

	write_bootblock(&imagefile, bootfile, boot_header, boot_program_header);

	/* Write image file signature */
	// Muda o ponteiro de escrita para o byte 510 do arquivo de imagem
	fseek(imagefile, BOOTLOADER_SIG_OFFSET, SEEK_SET);
	// Escreve a assinatura do bootloader no arquivo de imagem
	fwrite("\x55\xAA", 2, 1, imagefile);

	/* write kernel segments to image */

	write_kernel(&imagefile, kernelfile, kernel_header, kernel_program_header);

	/* tell the bootloader how many sectors to read to load the kernel */

	record_kernel_sectors(&imagefile, kernel_header, kernel_program_header, count_kernel_sectors(kernel_header, kernel_program_header));

	/* check for  --extended option */
	if (!strncmp(argv[1], "--extended", 11))
	{
		/* print info */
		extended_opt(boot_program_header, kernel_header->e_phnum, kernel_program_header, count_kernel_sectors(kernel_header, kernel_program_header));
	}

	return 0;
} // ends main()
