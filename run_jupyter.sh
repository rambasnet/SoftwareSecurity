# Check if Jupyter is installed
if ! command -v jupyter &> /dev/null
then
    echo "Jupyter not found. Installit it..."
    python3 -m pip install notebook
fi
# Start Jupyter Notebook
jupyter notebook --ip 0.0.0 --port 8888 --no-browser --allow-root
