#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>

static const char *dirpath = "/home/pepega/Downloads";

void encodeAtbash(char *str)
{
  // printf ("readdir atoz path: %s\n", path);

  int i = 0;
  while (str[i] != '\0' && str[i] != '.')
  {
    if (!((str[i] >= 0 && str[i] < 65) || (str[i] > 90 && str[i] < 97) || (str[i] > 122 && str[i] <= 127)))
    {
      if (str[i] >= 'A' && str[i] <= 'Z')
      {
        str[i] = 'Z' + 'A' - str[i];
        // printf("%c",'Z'+'A'-str[i]);
      }
      if (str[i] >= 'a' && str[i] <= 'z')
      {
        str[i] = 'z' + 'a' - str[i];
        // printf("%c",'z'+'a'-str[i]);
      }
    }
    // if(((str[i]>=0&&str[i]<65)||(str[i]>90&&str[i]<97)||(str[i]>122&&str[i]<=127))){
    //     printf("%c",str[i]);
    // }

    i++;
  }
  //strcat(str, ext);
}

void logging(char *call)
{
  FILE *log;
  log = fopen("/home/pepega/SinSeiFS.log", "a");
  char res[1000];

  fprintf(log, "%s\n", res);
  fclose(log);
}

static int xmp_getattr(const char *path, struct stat *stbuf)
{
  int res;
  char fpath[1000];

  sprintf(fpath, "%s%s", dirpath, path);

  res = lstat(fpath, stbuf);

  if (res == -1)
    return -errno;

  return 0;
}

static int xmp_mkdir(const char *path, mode_t mode)
{
  char fpath[1000];
  printf("path: %s\n", path);
  char *dirName = strstr(path, "AtoZ_");
  if (dirName != NULL)
  {
    // log
  }

  if (strcmp(path, "/") == 0)
  {
    path = dirpath;
    sprintf(fpath, "%s", path);
  }
  else
    sprintf(fpath, "%s%s", dirpath, path);

  int res = 0;
  res = mkdir(fpath, mode);

  if (res == -1)
  {
    return -errno;
  }

  // char* DESC = {fpath};
  // printf("mkdir: %s\n", DESC);

  return 0;
}

static int xmp_rename(const char *from, const char *to)
{
  printf("rename from:%s, to: %s\n", from, to);
  int res;

  res = rename(from, to);
  if (res == -1)
    return -errno;

  return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
  char fpath[1000];

  if (strcmp(path, "/") == 0)
  {
    path = dirpath;
    sprintf(fpath, "%s", path);
  }
  else
    sprintf(fpath, "%s%s", dirpath, path);

  int res = 0;

  DIR *dp;
  struct dirent *de;
  (void)offset;
  (void)fi;

  dp = opendir(fpath);

  if (dp == NULL)
    return -errno;

  while ((de = readdir(dp)) != NULL)
  {
    struct stat st;

    memset(&st, 0, sizeof(st));

    st.st_ino = de->d_ino;
    st.st_mode = de->d_type << 12;
    if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0)
    {
      res = (filler(buf, de->d_name, &st, 0));
      // continue;
    }
    if (strstr(path, "/AtoZ_"))
    {
      char filename[100];
      strcpy(filename, de->d_name);
      // char *ext;
      // ext = strrchr(filename, '.');
      // const char s[2] = ".";
      // char *token;
      // token = strtok(filename, s);
      // while (filename[i]!='\0'){
      // }
      // int subst = ext-filename;
      // printf("%d\n", subst);
      // printf("filename: %s ext:%s\n", token, ext);
      // char *enc = encodeAtbash(de->d_name);
      // printf("%s\n", enc);
      // printf("d_name: %s\n", de->d_name);
      encodeAtbash(filename);
      // printf("nama file: %s\n", filename);
      res = (filler(buf, filename, &st, 0));
    }
    else
    {
      res = (filler(buf, de->d_name, &st, 0));
      // if (filler(buf, de->d_name, & st, 0)) break;
    }
    if (res != 0)
      break;
  }

  closedir(dp);

  return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
  char fpath[1000];

  if (strcmp(path, "/") == 0)
  {
    path = dirpath;
    sprintf(fpath, "%s", path);
  }
  else
    sprintf(fpath, "%s%s", dirpath, path);

  int res = 0;
  int fd = 0;

  (void)fi;

  fd = open(fpath, O_RDONLY);

  if (fd == -1)
    return -errno;

  res = pread(fd, buf, size, offset);

  if (res == -1)
    res = -errno;

  close(fd);

  return res;
}

static int xmp_unlink(const char *path)
{
  char fpath[1000];

  if (strcmp(path, "/") == 0)
  {
    path = dirpath;
    sprintf(fpath, "%s", path);
  }
  else
    sprintf(fpath, "%s%s", dirpath, path);

  int res;

  res = unlink(fpath);
  if (res == -1)
    return -errno;

  return 0;
}

static int xmp_rmdir(const char *path)
{
  char fpath[1000];

  if (strcmp(path, "/") == 0)
  {
    path = dirpath;
    sprintf(fpath, "%s", path);
  }
  else
    sprintf(fpath, "%s%s", dirpath, path);

  int res;

  res = rmdir(fpath);
  if (res == -1)
    return -errno;

  return 0;
}

static int xmp_create(const char *path, mode_t mode, struct fuse_file_info *fi)
{
  (void)fi;
  char fpath[1000];

  if (strcmp(path, "/") == 0)
  {
    path = dirpath;
    sprintf(fpath, "%s", path);
  }
  else
    sprintf(fpath, "%s%s", dirpath, path);

  int res;
  res = creat(fpath, mode);
  if (res == -1)
    return -errno;

  close(res);

  return 0;
}

static struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .readdir = xmp_readdir,
    .mkdir = xmp_mkdir,
    .rmdir = xmp_rmdir,
    .read = xmp_read,
    .rename = xmp_rename,
    .create = xmp_create,
    .unlink = xmp_unlink,
};

int main(int argc, char *argv[])
{
  umask(0);

  return fuse_main(argc, argv, &xmp_oper, NULL);
}