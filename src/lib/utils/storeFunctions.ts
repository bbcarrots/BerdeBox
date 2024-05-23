import { Boxes } from "$lib/stores/IO";
import { getBoxByRef, getUserbyID } from "$lib/firebase/firestore";
import { getImages } from "$lib/firebase/storage";

export async function updateBoxesStore(boxRefs:any){

    let boxes:any[] = [];

    // Load user boxes and save to a local array
    for (const boxRef of boxRefs) {

        // Obtain the box, get the images of each box
        let box:any = await getBoxByRef(boxRef);
        const logs = await getImages(box.id);
        let reversedLogs = [...logs].reverse();

        boxes = [...boxes, {
            id: box.id,
            logs: reversedLogs
        }];
    }

    console.log("final boxes:", boxes);

    // Update the boxes store
    Boxes.set(boxes);
}

export async function isBoxInUserBoxes(boxID: number, userBoxes: any[]): Promise<boolean> {
    for (const boxRef of userBoxes) {
        const userBox:any = await getBoxByRef(boxRef);
        if (Number(userBox.id) === boxID) {
            console.log(userBox.id, boxID, Number(userBox.id) === boxID);
            return true;
        }
    }
    return false;
}