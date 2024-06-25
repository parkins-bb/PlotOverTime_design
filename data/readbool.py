import h5py

def check_hdf5_file(file_name):
    try:
        with h5py.File(file_name, 'r') as f:
            print("File contents:")
            f.visit(print)
            return True
    except Exception as e:
        print(f"Error reading HDF5 file: {e}")
        return False

# Check the file
check_hdf5_file('forward_step_2d_X1E4_100.h5')

