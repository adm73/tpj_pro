//
//  PreferencesViewController.swift
//  tpj453_GUI
//
//  Created by ChenYu on 2016-10-18.
//  Copyright © 2016 chyapp.com. All rights reserved.
//

import UIKit

class PreferencesTableViewCell: UITableViewCell {
    @IBOutlet weak var CurrValue: UILabel!
    @IBOutlet weak var MaxValue: UILabel!
    @IBOutlet weak var RangeSlider: UISlider!
    @IBOutlet weak var MinValue: UILabel!
    @IBOutlet weak var ControlName: UILabel!
    
    var action: ((_ sender: UISlider) -> Void)?
    @IBAction func RangeSliderValueChanged(_ sender: UISlider) {
        action?(sender)
    }
}

class PreferencesViewController: UITableViewController {
    
    var ContentArray = [
        "Light Sensor 1",
        "Light Sensor 2",
        "Soil Sensor",
        "Temp Sensor"
    ]
    
    var ContentDict: [String: Float] = [
        "Light Sensor 1": 500.0,
        "Light Sensor 2": 500.0,
        "Soil Sensor": 500.0,
        "Temp Sensor": 500.0
    ]

    override func viewDidLoad() {
        super.viewDidLoad()
        
        for(key, value) in ContentDict {
            print("\(key) : \(value)");
        }
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }

    // MARK: - Table view data source

    override func numberOfSections(in tableView: UITableView) -> Int {
        // #warning Incomplete implementation, return the number of sections
        return 1
    }

    override func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        // #warning Incomplete implementation, return the number of rows
        return ContentArray.count
    }

    
    override func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCell(withIdentifier: "PreferencesCell", for: indexPath) as! PreferencesTableViewCell
        
        cell.ControlName?.text = ContentArray[indexPath.row]
        
        cell.RangeSlider.maximumValue = 1000
        cell.MaxValue?.text = String(cell.RangeSlider.maximumValue)
        
        cell.RangeSlider.minimumValue  = 0
        cell.MinValue?.text = String(cell.RangeSlider.minimumValue)
        
        cell.RangeSlider.value  = 500
        cell.CurrValue?.text = String(cell.RangeSlider.value)
        
        cell.RangeSlider.tag = indexPath.row;
        
        cell.action = { [weak self] sender in
            if self != nil {
                cell.CurrValue?.text = String(cell.RangeSlider.value)
            }
        }
        return cell
    }
    
    /*
    // Override to support conditional editing of the table view.
    override func tableView(_ tableView: UITableView, canEditRowAt indexPath: IndexPath) -> Bool {
        // Return false if you do not want the specified item to be editable.
        return true
    }
    */

    /*
    // Override to support editing the table view.
    override func tableView(_ tableView: UITableView, commit editingStyle: UITableViewCellEditingStyle, forRowAt indexPath: IndexPath) {
        if editingStyle == .delete {
            // Delete the row from the data source
            tableView.deleteRows(at: [indexPath], with: .fade)
        } else if editingStyle == .insert {
            // Create a new instance of the appropriate class, insert it into the array, and add a new row to the table view
        }    
    }
    */

    /*
    // Override to support rearranging the table view.
    override func tableView(_ tableView: UITableView, moveRowAt fromIndexPath: IndexPath, to: IndexPath) {

    }
    */

    /*
    // Override to support conditional rearranging of the table view.
    override func tableView(_ tableView: UITableView, canMoveRowAt indexPath: IndexPath) -> Bool {
        // Return false if you do not want the item to be re-orderable.
        return true
    }
    */

    /*
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        // Get the new view controller using segue.destinationViewController.
        // Pass the selected object to the new view controller.
    }
    */

}
