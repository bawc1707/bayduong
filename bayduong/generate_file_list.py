import os

# Đường dẫn tới các thư mục
src_dir = "sources"
obj_dir = "obj"
header_dir = "headers"
source_extension = ".c"
object_extension = ".obj"

sources = []
objects = []

# Tạo thư mục obj nếu chưa tồn tại
if not os.path.exists(obj_dir):
    os.makedirs(obj_dir)

# Liệt kê các tệp nguồn và tệp object tương ứng
for filename in os.listdir(src_dir):
    if filename.endswith(source_extension):
        source_file = os.path.join(src_dir, filename)
        object_file = os.path.join(obj_dir, filename.replace(source_extension, object_extension))
        sources.append(source_file)
        objects.append(object_file)

# Ghi danh sách vào file_list.mk
with open("file_list.mk", "w") as f:
    f.write("SOURCES = " + " ".join(sources).replace("\\", "/") + "\n")
    f.write("OBJECTS = " + " ".join(objects).replace("\\", "/") + "\n")