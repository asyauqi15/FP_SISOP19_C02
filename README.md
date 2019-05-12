# FP_SISOP19_C02

## Fuse
```
static void* pre_init(struct fuse_conn_info *conn)
{
	system("rm -rfv /home/file_pindahan/");
	printf("coba\n");
	char huft[10240];
	sprintf(huft, "%s", "/home/pejuangsisop");
	push(huft);

    DIR *dp;
	struct dirent *de;

	while(front<=rear){
		printf("masuk while\n");
		char filename[10240];
		sprintf(filename, "%s/", queue_array[front]);
		dp = opendir(filename);

		while ((de = readdir(dp)) != NULL) {
			if (de->d_type == 4 && strcmp(de->d_name, ".") && strcmp(de->d_name, "..") && strcmp(de->d_name, "haha"))
			{
				char filename2[10240];
				sprintf(filename2, "%s/%s", queue_array[front], de->d_name);
				push(filename2);
			}
		}
		closedir(dp);
		pop();
	}

	front=0;

	while(front<=rear){
		char filename[10240];
		sprintf(filename, "%s/", queue_array[front]);
		dp = opendir(filename);

		while ((de = readdir(dp)) != NULL)
		{
			if(strcmp(de->d_name+strlen(de->d_name)-4, ".mp3")==0)
			{
				FILE *source, *target;
				char source_file[10240];
				char target_file[10240];
				sprintf(source_file, "%s%s", filename, de->d_name);
				sprintf(target_file, "/home/file_pindahan/%s", de->d_name);

				source = fopen(source_file, "rb");
				target = fopen(target_file, "wb");
				
				size_t n, m;
				unsigned char buff[8192];
				do {
					n = fread(buff, 1, sizeof buff, source);
					if (n) m = fwrite(buff, 1, n, target);
					else   m = 0;
				} while ((n > 0) && (n == m));
				
				fclose(source);
   				fclose(target);
			}
		}
		closedir(dp);
		pop();
	}

    return NULL;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{
	//front=0;
	
	DIR *dp;
	struct dirent *de;

	(void) offset;
	(void) fi;

	//while(front<=rear){
		char filename[10240];
		sprintf(filename, "%s%s", source, path);
		dp = opendir(filename);

		if (dp == NULL)
			return -errno;

		while ((de = readdir(dp)) != NULL)
		{
			if(strcmp(de->d_name+strlen(de->d_name)-4, ".mp3")==0)
			{
				printf("readdircoy\n%s\n", de->d_name);
				struct stat st;
				memset(&st, 0, sizeof(st));
				st.st_ino = de->d_ino;
				st.st_mode = de->d_type << 12;
				if (filler(buf, de->d_name, &st, 0)) break;
			}
		}
		closedir(dp);
		//pop();
	//}
	return 0;
}
```

Pada fuse, fungsi yang diubah adalah .init dan .readdir.<br><br>
Pada .init, ada beberapa langkah:
1. Menghapus semua file yang ada pada direktori temporary, disini saya menggunakan /home/file_pindahan
1. Traverse semua file berjenis direktory ke queue_array
1. Mencari setiap file yang berekstensi .mp3 di semua folder yang sudah ditraverse dan dicopy ke /home/file_pindahan

Lalu pada .readdir langkah yang dilakukan adalah:
1. Membuka folder /home/file_pindahan/
2. Filler semua file yang berekstensi .mp3

Berikut adalah hasilnya:
![fuse](/src/fuse.JPG)

## MP3 Player