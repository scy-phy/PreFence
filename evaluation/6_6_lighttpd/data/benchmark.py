#!/usr/bin/python3

import os
import sys
import json
import glob
import time
import datetime
import requests
from requests.adapters import HTTPAdapter
from urllib3.util import Retry

BASE_URL = "https://localhost:8443"
CONFIG_NAME = sys.argv[1] if len(sys.argv) > 1 else ""
# UPLOAD_FILE_PATHS = ["files/16M_0067108864.bin"]
UPLOAD_FILE_PATHS = glob.glob("files/*.bin")

session = requests.Session()
session.verify = "certs/ca.crt"
retries = Retry(
    total=10,
    backoff_factor=0.1,
    # status_forcelist=[502, 503, 504],
    # allowed_methods={'POST'},
)
session.mount('https://', HTTPAdapter(max_retries=retries))

def upload(upload_file_path):
    upload_file_basename = os.path.basename(upload_file_path)
    
    # remove old instances of the file from the server
    session.get(f"{BASE_URL}/remove.php", params={"file": upload_file_basename})
    
    # upload the file
    time_upload_ns_before = time.time_ns()
    upload_result = session.post(
        f"{BASE_URL}/upload.php",
        files={
            "file": (
                upload_file_basename,
                open(upload_file_path, "rb").read(),
                "application/octet-stream",
            )
        },
    )
    time_upload_ns_after = time.time_ns()
    time_upload_s = (time_upload_ns_after - time_upload_ns_before) / 10**9
    
    if upload_result.status_code != 200:
        print(f"Upload failed with status {upload_result.status_code})")
        print(upload_result.text)
        sys.exit(1)
    
    # remove file from the server
    session.get(f"{BASE_URL}/remove.php", params={"file": upload_file_basename})

    return time_upload_s

#####

experiment_results = []
for upload_file_path in UPLOAD_FILE_PATHS:
    print(f"# Processing {upload_file_path}...")

    time_upload_s = upload(upload_file_path)
    print(f"Time Upload: {time_upload_s}")
    
    experiment_result = {
        "upload_file_path": upload_file_path,
        "time_upload_s": time_upload_s,
    }
    experiment_results.append(experiment_result)

with open(f"results_{CONFIG_NAME}_{datetime.datetime.now().strftime('%Y%m%d-%H%M%S')}.json", "w") as results_file:
    results_file.write(json.dumps(experiment_results, indent=2))
