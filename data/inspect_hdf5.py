import os
import h5py
import sys

def print_hdf5_structure(file_name):
    def print_attrs(name, obj):
        print(f"{name}")
        for key, val in obj.attrs.items():
            print(f"    {key}: {val}")
    
    with h5py.File(file_name, 'r') as f:
        print(f"File: {file_name}")
        f.visititems(print_attrs)
        
        print("\nDatasets:")
        def print_dataset(name, obj):
            if isinstance(obj, h5py.Dataset):
                print(f"Dataset: {name}, shape: {obj.shape}, dtype: {obj.dtype}")
        
        f.visititems(print_dataset)
        
if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python inspect_hdf5.py <path_to_h5_file>")
        sys.exit(1)

    file_name = sys.argv[1]
    if not os.path.exists(file_name):
        print(f"File {file_name} does not exist.")
        sys.exit(1)

    print_hdf5_structure(file_name)

