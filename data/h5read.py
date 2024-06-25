import h5py

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
    file_name = 'forward_step_2d_X1E4_100.h5'
    print_hdf5_structure(file_name)

