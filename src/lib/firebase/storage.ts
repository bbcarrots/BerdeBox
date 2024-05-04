import { firebaseApp } from './config';
import { getStorage, ref, getDownloadURL } from 'firebase/storage';

const storage = getStorage(firebaseApp);

//function to get image from firebase and returns the image source
export async function getImage(imageName: string): Promise<string | null> {
    try {
        //path is images/[imageName]
        const storageRef = ref(storage, "images/" + imageName);
        const url = await getDownloadURL(storageRef);
        return url;
    } catch (error) {
        console.error("Error getting image:", error);
        return null;
    }
}